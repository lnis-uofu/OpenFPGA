//////////////////////////////////////////////////////////////////////
// Created by SmartDesign Tue Jan 16 17:22:21 2018
// Version: v11.8 11.8.0.26
//////////////////////////////////////////////////////////////////////

`timescale 1ns / 100ps

// TOP_multi_enc_decx2x4
module  multi_enc_decx2x4(
    // Inputs
    clock,
    datain,
    datain1,
    datain1_0,
    datain_0,
    reset,
    // Outputs
    dataout,
    dataout1,
    dataout1_0,
    dataout_0
);

//--------------------------------------------------------------------
// Input
//--------------------------------------------------------------------
input          clock;
input  [127:0] datain;
input  [127:0] datain1;
input  [127:0] datain1_0;
input  [127:0] datain_0;
input          reset;
//--------------------------------------------------------------------
// Output
//--------------------------------------------------------------------
output [127:0] dataout;
output [127:0] dataout1;
output [127:0] dataout1_0;
output [127:0] dataout_0;
//--------------------------------------------------------------------
// Nets
//--------------------------------------------------------------------
wire           clock;
wire   [127:0] datain;
wire   [127:0] datain1;
wire   [127:0] datain1_0;
wire   [127:0] datain_0;
wire   [127:0] dataout_net_0;
wire   [127:0] dataout1_net_0;
wire   [127:0] dataout1_0_net_0;
wire   [127:0] dataout_0_net_0;
wire           reset;
wire   [127:0] top_0_dataout;
wire   [127:0] top_1_dataout1;
wire   [127:0] dataout_net_1;
wire   [127:0] dataout1_net_1;
wire   [127:0] dataout1_0_net_1;
wire   [127:0] dataout_0_net_1;
//--------------------------------------------------------------------
// Top level output port assignments
//--------------------------------------------------------------------
assign dataout_net_1     = dataout_net_0;
assign dataout[127:0]    = dataout_net_1;
assign dataout1_net_1    = dataout1_net_0;
assign dataout1[127:0]   = dataout1_net_1;
assign dataout1_0_net_1  = dataout1_0_net_0;
assign dataout1_0[127:0] = dataout1_0_net_1;
assign dataout_0_net_1   = dataout_0_net_0;
assign dataout_0[127:0]  = dataout_0_net_1;
//--------------------------------------------------------------------
// Component instances
//--------------------------------------------------------------------
//--------top
top top_0(
        // Inputs
        .clock    ( clock ),
        .reset    ( reset ),
        .datain   ( datain ),
        .datain1  ( datain1 ),
        // Outputs
        .dataout  ( top_0_dataout ),
        .dataout1 ( dataout1_0_net_0 ) 
        );

//--------top
top top_1(
        // Inputs
        .clock    ( clock ),
        .reset    ( reset ),
        .datain   ( datain_0 ),
        .datain1  ( datain1_0 ),
        // Outputs
        .dataout  ( dataout_0_net_0 ),
        .dataout1 ( top_1_dataout1 ) 
        );

//--------top
top top_2(
        // Inputs
        .clock    ( clock ),
        .reset    ( reset ),
        .datain   ( top_0_dataout ),
        .datain1  ( top_1_dataout1 ),
        // Outputs
        .dataout  ( dataout_net_0 ),
        .dataout1 ( dataout1_net_0 ) 
        );


endmodule
