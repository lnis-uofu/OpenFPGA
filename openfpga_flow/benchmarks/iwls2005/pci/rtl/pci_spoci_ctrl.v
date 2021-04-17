//////////////////////////////////////////////////////////////////////
////                                                              ////
////  File name: pci_spoci_ctrl                                   ////
////                                                              ////
////  This file is part of the "PCI bridge" project               ////
////  http://www.opencores.org/cores/pci/                         ////
////                                                              ////
////  Author(s):                                                  ////
////      - Miha Dolenc (mihad@opencores.org)                     ////
////                                                              ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
////                                                              ////
//// Copyright (C) 2004 Miha Dolenc, mihad@opencores.org          ////
////                                                              ////
//// This source file may be used and distributed without         ////
//// restriction provided that this copyright statement is not    ////
//// removed from the file and that any derivative work contains  ////
//// the original copyright notice and the associated disclaimer. ////
////                                                              ////
//// This source file is free; you can redistribute it            ////
//// and/or modify it under the terms of the GNU Lesser General   ////
//// Public License as published by the Free Software Foundation; ////
//// either version 2.1 of the License, or (at your option) any   ////
//// later version.                                               ////
////                                                              ////
//// This source is distributed in the hope that it will be       ////
//// useful, but WITHOUT ANY WARRANTY; without even the implied   ////
//// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR      ////
//// PURPOSE.  See the GNU Lesser General Public License for more ////
//// details.                                                     ////
////                                                              ////
//// You should have received a copy of the GNU Lesser General    ////
//// Public License along with this source; if not, download it   ////
//// from http://www.opencores.org/lgpl.shtml                     ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//
// CVS Revision History
//
// $Log: pci_spoci_ctrl.v,v $
// Revision 1.1  2004/01/24 11:54:18  mihad
// Update! SPOCI Implemented!
//
//

`include "pci_constants.v"

// synopsys translate_off
`include "timescale.v"
// synopsys translate_on

module pci_spoci_ctrl
(
    reset_i             ,
    clk_i               ,
                        
    do_rnd_read_i       ,
    do_seq_read_i       ,
    do_write_i          ,
                        
    write_done_o        ,
    dat_rdy_o           ,
    no_ack_o            ,
                        
    adr_i               ,
    dat_i               ,
    dat_o               ,

    pci_spoci_sda_i     ,
    pci_spoci_sda_oe_o  ,
    pci_spoci_scl_oe_o
);

parameter tx_rx_state_width = 9     ;
parameter tx_rx_idle        = 'h1   ;
parameter tx_rx_start       = 'h2   ;
parameter tx_rx_restart     = 'h4   ;
parameter tx_rx_send_bits   = 'h8   ;
parameter tx_rx_rec_bits    = 'h10  ;
parameter tx_rx_send_ack    = 'h20  ;
parameter tx_rx_rec_ack     = 'h40  ;
parameter tx_rx_send_nack   = 'h80  ;
parameter tx_rx_stop        = 'h100 ;

parameter rw_seq_state_width    = 5     ;
parameter rw_seq_idle           = 'h1   ;
parameter rw_seq_tx_ctrl        = 'h2   ;
parameter rw_seq_tx_adr         = 'h4   ;
parameter rw_seq_tx_byte        = 'h8   ;
parameter rw_seq_rx_byte        = 'h10  ;

`ifdef PCI33
parameter cnt_width     = 9     ;
parameter period_cnt    = 334   ;
`endif

`ifdef PCI66
parameter cnt_width     = 10    ;
parameter period_cnt    = 667   ;
`endif

input   reset_i ,
        clk_i   ;

input   do_rnd_read_i   ,
        do_seq_read_i   ,
        do_write_i      ;

output  write_done_o    ,
        dat_rdy_o       ,
        no_ack_o        ;

input   [10: 0] adr_i   ;
input   [ 7: 0] dat_i   ;
output  [ 7: 0] dat_o   ;

input   pci_spoci_sda_i     ;

output  pci_spoci_sda_oe_o  ,
        pci_spoci_scl_oe_o  ;

reg write_done_o    ,
    dat_rdy_o       ,
    no_ack_o        ;

reg [ 7: 0] dat_o   ;

reg pci_spoci_sda_oe_o  ,
    pci_spoci_scl_oe_o  ;

reg clk_gen_cnt_en  ;
reg clk_gen_cnt_clr ;
reg [cnt_width - 1:0] clk_gen_cnt   ;

reg [tx_rx_state_width - 1:0] tx_rx_state       ;
reg [tx_rx_state_width - 1:0] tx_rx_next_state  ;
reg tx_rx_sm_idle ;

reg scl_oe      ;
reg scl_oe_en   ;
reg sda_oe      ;
reg sda_oe_en   ;

reg sda_i_reg_en    ;
reg sda_i_reg       ;

always@(posedge clk_i or posedge reset_i)
begin
    if (reset_i)
    begin
        clk_gen_cnt <= 'h0  ;

        tx_rx_state <= tx_rx_idle   ;

    `ifdef ACTIVE_LOW_OE
        pci_spoci_sda_oe_o <= 1'b1 ;
        pci_spoci_scl_oe_o <= 1'b1 ;
    `endif

    `ifdef ACTIVE_HIGH_OE
        pci_spoci_sda_oe_o <= 1'b0 ;
        pci_spoci_scl_oe_o <= 1'b0 ;
    `endif

        sda_i_reg <= 1'b1 ;
    end
    else
    begin
        tx_rx_state <= tx_rx_next_state ;
        
        if (clk_gen_cnt_clr)
            clk_gen_cnt <= 'h0  ;
        else if (clk_gen_cnt_en)
            clk_gen_cnt <= clk_gen_cnt + 1'b1 ;

    
        if (sda_oe_en)
        begin
        `ifdef ACTIVE_LOW_OE
            pci_spoci_sda_oe_o <= ~sda_oe   ;
        `endif

        `ifdef ACTIVE_HIGH_OE
            pci_spoci_sda_oe_o <= sda_oe    ;
        `endif
        end

        if (scl_oe_en)
        begin
        `ifdef ACTIVE_LOW_OE
            pci_spoci_scl_oe_o <= ~scl_oe   ;
        `endif

        `ifdef ACTIVE_HIGH_OE
            pci_spoci_scl_oe_o <= scl_oe    ;
        `endif
        end

        if (sda_i_reg_en)
            sda_i_reg <= pci_spoci_sda_i ;
    end
end

reg [ 7: 0] tx_shift_reg ;

reg send_start  ;
reg start_sent  ;

reg send_bit    ;
reg bit_sent    ;

reg rec_bit     ;
reg bit_rec     ;

reg rec_ack     ;
reg ack_rec     ;
reg nack_rec    ;

reg send_ack    ;
reg ack_sent    ;
reg send_nack   ;
reg nack_sent   ;

reg send_stop   ;
reg stop_sent   ;

always@
(
    tx_rx_state     or
    clk_gen_cnt     or
    send_start      or
    send_bit        or
    tx_shift_reg    or
    send_stop       or
    rec_ack         or
    sda_i_reg       or
    rec_bit         or
    send_ack        or
    send_nack
)
begin
    clk_gen_cnt_clr     = 1'b0          ;
    clk_gen_cnt_en      = 1'b0          ;
    tx_rx_next_state    = tx_rx_state   ;
    tx_rx_sm_idle       = 1'b0          ;
    scl_oe              = 1'b0          ;
    sda_oe              = 1'b0          ;
    scl_oe_en           = 1'b0          ;
    sda_oe_en           = 1'b0          ;
    start_sent          = 1'b0          ;
    bit_sent            = 1'b0          ;
    ack_rec             = 1'b0          ;
    nack_rec            = 1'b0          ;
    sda_i_reg_en        = 1'b0          ;
    stop_sent           = 1'b0          ;
    bit_rec             = 1'b0          ;
    ack_sent            = 1'b0          ;
    nack_sent           = 1'b0          ;

    case (tx_rx_state)

    tx_rx_idle:
    begin
        tx_rx_sm_idle = 1'b1 ;

        // from idle state, the only transition can be to the send start bit
        if (send_start)
        begin
            tx_rx_next_state = tx_rx_start  ;
            clk_gen_cnt_clr  = 1'b1         ;
        end
    end

    tx_rx_start:
    begin
        clk_gen_cnt_en  = 1'b1  ;
        sda_oe          = 1'b1  ;

        // start bit is sent by transmiting 0 on the sda line
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            start_sent = 1'b1 ;
            sda_oe_en  = 1'b1 ;
        end
        
        // after half clock period of driving the sda low, the only possible
        // transition is to send state.
        // if send bit is not active, stop the procedure - undrive sda
        if (clk_gen_cnt == period_cnt)
        begin
            clk_gen_cnt_clr = 1'b1 ;
            if (send_bit)
            begin
                tx_rx_next_state = tx_rx_send_bits ;
            end
            else
            begin
                sda_oe              = 1'b0          ;
                sda_oe_en           = 1'b1          ;
                tx_rx_next_state    = tx_rx_idle    ;
            end
        end
    end

    tx_rx_send_bits:
    begin
        clk_gen_cnt_en = 1'b1 ;

        // generate high to low transition on the scl line immediately
        if (clk_gen_cnt == 'h0)
        begin
            scl_oe      = 1'b1  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock low time, load new value for sda oe, depending on the
        // msb bit in the shift register
        if (clk_gen_cnt == (period_cnt >> 2))
        begin
            sda_oe      = ~tx_shift_reg[7]  ;
            sda_oe_en   = 1'b1              ;
            bit_sent    = 1'b1              ;
        end

        // after clock low time, generate low to high transition on the scl line
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            scl_oe      = 1'b0  ;
            scl_oe_en   = 1'b1  ;
        end

        // after clock high time, check what to do next
        if (clk_gen_cnt == (period_cnt))
        begin
            clk_gen_cnt_clr = 1'b1 ;

            if (~send_bit)
            begin
                // after transmiting all the bits, the only possible transition is to the state
                // that checks the eprom acknowledge
                if (rec_ack)
                    tx_rx_next_state = tx_rx_rec_ack ;
                else
                begin
                    sda_oe              = 1'b0          ;
                    sda_oe_en           = 1'b1          ;
                    tx_rx_next_state    = tx_rx_idle    ;
                end
            end
        end
    end

    tx_rx_rec_bits:
    begin
        clk_gen_cnt_en = 1'b1 ;
        sda_i_reg_en   = 1'b1 ;

        // generate high to low transition on the scl line immediately
        if (clk_gen_cnt == 'h0)
        begin
            scl_oe      = 1'b1  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock low time, disable sda driver
        if (clk_gen_cnt == (period_cnt >> 2))
        begin
            sda_oe      = 1'b0  ;
            sda_oe_en   = 1'b1  ;
        end

        // after clock low time, generate low to high transition on the scl line
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            scl_oe      = 1'b0  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock high time, report received bit
        if (clk_gen_cnt == ((period_cnt >> 1) + (period_cnt >> 2)) )
        begin
            bit_rec = 1'b1  ;
        end

        // after clock period is finished, check the next operation
        if (clk_gen_cnt == (period_cnt))
        begin
            clk_gen_cnt_clr = 1'b1 ;

            if (~rec_bit)
            begin
                // when all bits are received, only nack or ack next states are possible
                if (send_ack)
                    tx_rx_next_state = tx_rx_send_ack   ;
                else if (send_nack)
                    tx_rx_next_state = tx_rx_send_nack  ;
                else
                begin
                    tx_rx_next_state = tx_rx_idle    ;
                end
            end
        end
    end

    tx_rx_send_ack:
    begin
        clk_gen_cnt_en  = 1'b1  ;

        // generate high to low transition on the scl line
        if (clk_gen_cnt == 'h0)
        begin
            scl_oe      = 1'b1  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock low time, enable the sda driver
        if (clk_gen_cnt == (period_cnt >> 2))
        begin
            sda_oe      = 1'b1  ;
            sda_oe_en   = 1'b1  ;
            ack_sent    = 1'b1  ;
        end

        // after clock low time, disable the scl driver - generate low to high transition on the scl line
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            scl_oe      = 1'b0  ;
            scl_oe_en   = 1'b1  ;
        end

        // after clock period time expires, check what to do next
        if (clk_gen_cnt == period_cnt)
        begin
            clk_gen_cnt_clr = 1'b1 ;

            // after the byte is acknowledged, the only possible next state is receive bits
            // state
            if (rec_bit)
                tx_rx_next_state = tx_rx_rec_bits   ;
            else
            begin
                // this should never happen
                sda_oe      = 1'b0  ;
                sda_oe_en   = 1'b1  ;

                tx_rx_next_state = tx_rx_idle ;
            end
        end
    end

    tx_rx_rec_ack:
    begin

        clk_gen_cnt_en  = 1'b1  ;
        sda_i_reg_en    = 1'b1  ;

        // generate high to low transition on the scl line
        if (clk_gen_cnt == 'h0)
        begin
            scl_oe      = 1'b1  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock low time, disable the sda driver
        if (clk_gen_cnt == (period_cnt >> 2))
        begin
            sda_oe      = 1'b0  ;
            sda_oe_en   = 1'b1  ;
        end

        // after clock low time, disable the scl driver - generate low to high transition on the scl line
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            scl_oe      = 1'b0  ;
            scl_oe_en   = 1'b1  ;
        end

        // after 1/2 clock high time, report ack or nack condition, depending on the sda input state
        if (clk_gen_cnt == ((period_cnt >> 1) + (period_cnt >> 2)) )
        begin
            ack_rec     = ~sda_i_reg ;
            nack_rec    =  sda_i_reg ;
        end

        // after clock period time expires, check what to do next
        if (clk_gen_cnt == period_cnt)
        begin
            clk_gen_cnt_clr = 1'b1 ;

            if (send_bit)
                tx_rx_next_state = tx_rx_send_bits  ;
            else if (rec_bit)
                tx_rx_next_state = tx_rx_rec_bits   ;
            else if (send_stop)
                tx_rx_next_state = tx_rx_stop       ;
            else if (send_start)
                tx_rx_next_state = tx_rx_restart    ;
            else
            begin
                // this should never happen
                tx_rx_next_state = tx_rx_idle ;
            end
        end
    end

    tx_rx_send_nack:
    begin
        clk_gen_cnt_en  = 1'b1  ;

        // generate high to low transition on the scl line
        if (clk_gen_cnt == 'h0)
        begin
            scl_oe      = 1'b1  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock low time, disable the sda driver
        if (clk_gen_cnt == (period_cnt >> 2))
        begin
            sda_oe      = 1'b0  ;
            sda_oe_en   = 1'b1  ;
            nack_sent   = 1'b1  ;
        end

        // after clock low time, disable the scl driver - generate low to high transition on the scl line
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            scl_oe      = 1'b0  ;
            scl_oe_en   = 1'b1  ;
        end

        // after clock period time expires, check what to do next
        if (clk_gen_cnt == period_cnt)
        begin
            clk_gen_cnt_clr = 1'b1 ;

            // after the no acknowledge is sent, the only possible next state is stop
            // state
            if (send_stop)
                tx_rx_next_state = tx_rx_stop   ;
            else
            begin
                // this should never happen
                tx_rx_next_state = tx_rx_idle ;
            end
        end
    end

    tx_rx_restart:
    begin
        clk_gen_cnt_en = 1'b1 ;

        // generate high to low transition
        if (clk_gen_cnt == 'h0)
        begin
            scl_oe      = 1'b1  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock low time, release sda line
        if (clk_gen_cnt == (period_cnt >> 2))
        begin
            sda_oe      = 1'b0  ;
            sda_oe_en   = 1'b1  ;
        end

        // generate low to high transition
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            clk_gen_cnt_clr = 1'b1 ;

            scl_oe      = 1'b0  ;
            scl_oe_en   = 1'b1  ;

            if (send_start)
                tx_rx_next_state = tx_rx_start ;
            else
                tx_rx_next_state = tx_rx_idle ;
        end
    end

    tx_rx_stop:
    begin
        clk_gen_cnt_en = 1'b1 ;

        // generate high to low transition
        if (clk_gen_cnt == 'h0)
        begin
            scl_oe      = 1'b1  ;
            scl_oe_en   = 1'b1  ;
        end

        // after half of clock low time, drive sda line low
        if (clk_gen_cnt == (period_cnt >> 2))
        begin
            sda_oe      = 1'b1  ;
            sda_oe_en   = 1'b1  ;
        end

        // generate low to high transition
        if (clk_gen_cnt == (period_cnt >> 1))
        begin
            scl_oe      = 1'b0  ;
            scl_oe_en   = 1'b1  ;
        end

        // after full clock period, release the sda line
        if (clk_gen_cnt == period_cnt)
        begin
            sda_oe      = 1'b0  ;
            sda_oe_en   = 1'b1  ;
            stop_sent   = 1'b1  ;

            tx_rx_next_state = tx_rx_idle ;
        end
    end

    endcase
end

reg [rw_seq_state_width - 1:0]  rw_seq_state    ;

reg doing_read      , 
    doing_write     , 
    doing_seq_read  ,
    adr_set         ;

reg [ 3: 0] bits_transfered ;

always@(posedge clk_i or posedge reset_i)
begin
    if (reset_i)
    begin
        rw_seq_state    <= rw_seq_idle  ;
        adr_set         <= 1'b0         ;
        doing_read      <= 1'b0         ;
        doing_write     <= 1'b0         ;
        doing_seq_read  <= 1'b0         ;
        dat_o           <= 'h0          ;
        tx_shift_reg    <= 'h0          ;
        send_start      <= 'h0          ;
        send_stop       <= 'h0          ;
        send_bit        <= 'h0          ;
        send_nack       <= 'h0          ;
        rec_ack         <= 'h0          ;
        no_ack_o        <= 'h0          ;
        bits_transfered <= 'h0          ;
        write_done_o    <= 'h0          ;
        dat_rdy_o       <= 'h0          ;
        send_ack        <= 'h0          ;
        rec_bit         <= 'h0          ;
    end
    else
    begin

        case (rw_seq_state)
            
        rw_seq_idle:
        begin
            tx_shift_reg <= {4'b1010, adr_i[10: 8], 1'b0} ;
            adr_set      <= 1'b0                          ;

            if ( tx_rx_sm_idle & ~(doing_write | doing_read | doing_seq_read) )
            begin
                if (do_write_i | do_rnd_read_i | do_seq_read_i)
                begin
                    rw_seq_state <= rw_seq_tx_ctrl  ;
                    send_start   <= 1'b1            ;
                end

                if (do_write_i)
                    doing_write     <= 1'b1 ;
                else if (do_rnd_read_i)
                    doing_read      <= 1'b1 ;
                else if (do_seq_read_i)
                    doing_seq_read  <= 1'b1 ;
            end
            else
            begin
                doing_write     <= 1'b0 ;
                doing_read      <= 1'b0 ;
                doing_seq_read  <= 1'b0 ;
            end
        end
    
        rw_seq_tx_ctrl:
        begin
            if (send_start)
            begin
                bits_transfered <= 'h0 ;

                if (start_sent)
                begin
                    send_start <= 1'b0 ;
                    send_bit   <= 1'b1 ;
                end
            end
            else if (send_bit)
            begin
                if (bit_sent)
                begin
                    bits_transfered <= bits_transfered + 1'b1 ;
                    tx_shift_reg <= {tx_shift_reg[6:0], tx_shift_reg[0]} ;
                end

                if (bits_transfered == 'h8)
                begin
                    send_bit <= 1'b0 ;
                    rec_ack  <= 1'b1 ;
                end
            end
            else if (rec_ack)
            begin
                bits_transfered <= 'h0 ;

                if (ack_rec | nack_rec)
                    rec_ack <= 1'b0 ;

                if (ack_rec)
                begin
                    if (doing_write | ~adr_set)
                    begin
                        rw_seq_state    <= rw_seq_tx_adr    ;
                        tx_shift_reg    <= adr_i[ 7: 0]     ;
                        send_bit        <= 1'b1             ;
                    end
                    else
                    begin
                        rw_seq_state    <= rw_seq_rx_byte   ;
                        rec_bit         <= 1'b1             ;
                    end
                end
                else if (nack_rec)
                begin
                    no_ack_o    <= 1'b1 ;
                    send_stop   <= 1'b1 ;
                end
            end
            else if (send_stop)
            begin
                no_ack_o <= 1'b0 ;

                if (stop_sent)
                begin
                    send_stop       <= 1'b0         ;
                    rw_seq_state    <= rw_seq_idle  ;
                end
            end
        end
    
        rw_seq_tx_adr:
        begin
            if (send_bit)
            begin
                if (bit_sent)
                begin
                    bits_transfered <= bits_transfered + 1'b1 ;
                    tx_shift_reg <= {tx_shift_reg[6:0], tx_shift_reg[0]} ;
                end

                if (bits_transfered == 'h8)
                begin
                    send_bit <= 1'b0 ;
                    rec_ack  <= 1'b1 ;
                end
            end
            else if (rec_ack)
            begin
                bits_transfered <= 'h0 ;

                if (ack_rec | nack_rec)
                    rec_ack <= 1'b0 ;

                if (ack_rec)
                begin

                    adr_set <= 1'b1 ;

                    if (doing_write)
                    begin
                        send_bit        <= 1'b1             ;
                        rw_seq_state    <= rw_seq_tx_byte   ;
                        tx_shift_reg    <= dat_i            ;
                    end
                    else if (doing_read | doing_seq_read)
                    begin
                        send_start      <= 1'b1             ;
                        rw_seq_state    <= rw_seq_tx_ctrl   ;
                        tx_shift_reg    <= 8'b10100001      ;
                    end
                end
                else if (nack_rec)
                begin
                    no_ack_o    <= 1'b1 ;
                    send_stop   <= 1'b1 ;
                end
            end
            else if (send_stop)
            begin
                no_ack_o    <= 1'b0 ;

                if (stop_sent)
                begin
                    send_stop       <= 1'b0         ;
                    rw_seq_state    <= rw_seq_idle  ;
                end
            end
        end
    
        rw_seq_tx_byte:
        begin
            if (send_bit)
            begin
                if (bit_sent)
                begin
                    bits_transfered <= bits_transfered + 1'b1 ;
                    tx_shift_reg <= {tx_shift_reg[6:0], tx_shift_reg[0]} ;
                end

                if (bits_transfered == 'h8)
                begin
                    send_bit <= 1'b0 ;
                    rec_ack  <= 1'b1 ;
                end
            end
            else if (rec_ack)
            begin
                bits_transfered <= 'h0 ;

                if (ack_rec | nack_rec)
                begin
                    rec_ack   <= 1'b0   ;
                    send_stop <= 1'b1   ;
                end

                if (nack_rec)
                    no_ack_o <= 1'b1 ;

                if (ack_rec)
                    write_done_o <= 1'b1 ;
            end
            else if (send_stop)
            begin
                no_ack_o        <= 1'b0 ;
                write_done_o    <= 1'b0 ;

                if (stop_sent)
                begin
                    send_stop       <= 1'b0         ;
                    rw_seq_state    <= rw_seq_idle  ;
                end
            end
        end
    
        rw_seq_rx_byte:
        begin
            if (rec_bit)
            begin
                if (bit_rec)
                begin
                    bits_transfered <= bits_transfered + 1'b1   ;
                    dat_o           <= {dat_o[6:0], sda_i_reg}  ;
                end

                if (bits_transfered == 'h8)
                begin
                    rec_bit   <= 1'b0 ;
                    dat_rdy_o <= 1'b1 ;
                    if (doing_read)
                        send_nack <= 1'b1 ;
                    else
                        send_ack  <= 1'b1 ;
                end
            end
            else if (send_nack)
            begin
                dat_rdy_o       <= 1'b0 ;
                bits_transfered <= 'h0  ;

                if (nack_sent)
                begin
                    send_stop <= 1'b1 ;
                    send_nack <= 1'b0 ;
                end
            end
            else if (send_ack)
            begin
                dat_rdy_o       <= 1'b0 ;
                bits_transfered <= 'h0  ;

                if (~do_seq_read_i)
                begin
                    send_ack    <= 1'b0 ;
                    send_nack   <= 1'b1 ;
                end
                else if (ack_sent)
                begin
                    send_ack <= 1'b0 ;
                    rec_bit  <= 1'b1 ;
                end
            end
            else if (send_stop)
            begin
                if (stop_sent)  
                begin
                    send_stop       <= 1'b0         ;
                    rw_seq_state    <= rw_seq_idle  ;
                end
            end
        end
        endcase
    end
end

endmodule // pci_spoci_ctrl