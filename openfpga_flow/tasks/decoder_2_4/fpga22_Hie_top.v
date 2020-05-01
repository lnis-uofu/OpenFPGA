

module fpga_top
(
  // inout DVDD_pad,
  // inout DVSS_pad,
  input [0:0] pReset_pad,
  input [0:0] prog_clk_pad,
  input [0:0] Reset_pad,
  input [0:0] Test_en_pad,
  input [0:0] clk_pad,
  input [0:0] sc_head_pad,
  output [0:0] sc_tail_pad,
  inout [7:0] gpio_pad,
  input [0:0] ccff_head_pad,
  output [0:0] ccff_tail_pad
);

  wire [0:0] pReset;
  wire [0:0] prog_clk;
  wire [0:0] Reset;
  wire [0:0] Test_en;
  wire [0:0] clk;
  wire [0:0] sc_head;
  wire [0:0] sc_tail;
  wire [7:0] gfpga_pad_GPIO_A;
  wire [7:0] gfpga_pad_GPIO_IE;
  wire [7:0] gfpga_pad_GPIO_OE;
  wire [7:0] gfpga_pad_GPIO_Y;
  wire [0:0] ccff_head;
  wire [0:0] ccff_tail;
  wire DVDD;
  wire DVSS;
  wire SNS;
  wire RTO;
  wire [65:0] TIELOW;
  wire [28:0] TIEHIGH;
  wire [18:0] UNCONN;
  assign DVDD_pad = DVDD;
  assign DVSS_pad = DVSS;

  fpga_core
  fpga_core_uut
  (
    .pReset(pReset),
    .prog_clk(prog_clk),
    .Reset(Reset),
    .Test_en(Test_en),
    .clk(clk),
    .sc_head(sc_head),
    .sc_tail(sc_tail),
    .gfpga_pad_GPIO_A(gfpga_pad_GPIO_A),
    .gfpga_pad_GPIO_IE(gfpga_pad_GPIO_IE),
    .gfpga_pad_GPIO_OE(gfpga_pad_GPIO_OE),
    .gfpga_pad_GPIO_Y(gfpga_pad_GPIO_Y),
    .ccff_head(ccff_head),
    .ccff_tail(ccff_tail)
  );


  PDVSS_18_18_NT_DR_H
  PDVSS_18_18_NT_DR_left_cell_0
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PDVDD_18_18_NT_DR_H
  PDVDD_18_18_NT_DR_left_cell_1
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PINCNP_18_18_NT_DR_H
  PINCNP_18_18_NT_DR_left_cell_2
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(Reset_pad),
    .Y(Reset),
    .IE(TIEHIGH[0]),
    .IS(TIELOW[0]),
    .POE(TIELOW[1]),
    .PO(UNCONN[0])
  );


  PBIDIR_18_18_NT_DR_H
  PBIDIR_18_18_NT_DR_left_cell_3
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[6]),
    .Y(gfpga_pad_GPIO_Y[6]),
    .A(gfpga_pad_GPIO_A[6]),
    .IE(gfpga_pad_GPIO_IE[6]),
    .OE(gfpga_pad_GPIO_OE[6]),
    .DS0(TIEHIGH[1]),
    .DS1(TIEHIGH[2]),
    .IS(TIELOW[2]),
    .PE(TIELOW[3]),
    .POE(TIELOW[4]),
    .PS(TIELOW[5]),
    .SR(TIELOW[6]),
    .PO(UNCONN[1])
  );


  PINCNP_18_18_NT_DR_H
  PINCNP_18_18_NT_DR_left_cell_4
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(clk_pad),
    .Y(clk),
    .IE(TIEHIGH[3]),
    .IS(TIELOW[7]),
    .POE(TIELOW[8]),
    .PO(UNCONN[2])
  );


  PBIDIR_18_18_NT_DR_H
  PBIDIR_18_18_NT_DR_left_cell_5
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[7]),
    .Y(gfpga_pad_GPIO_Y[7]),
    .A(gfpga_pad_GPIO_A[7]),
    .IE(gfpga_pad_GPIO_IE[7]),
    .OE(gfpga_pad_GPIO_OE[7]),
    .DS0(TIEHIGH[4]),
    .DS1(TIEHIGH[5]),
    .IS(TIELOW[9]),
    .PE(TIELOW[10]),
    .POE(TIELOW[11]),
    .PS(TIELOW[12]),
    .SR(TIELOW[13]),
    .PO(UNCONN[3])
  );


  PINCNP_18_18_NT_DR_H
  PINCNP_18_18_NT_DR_left_cell_6
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(Test_en_pad),
    .Y(Test_en),
    .IE(TIEHIGH[6]),
    .IS(TIELOW[14]),
    .POE(TIELOW[15]),
    .PO(UNCONN[4])
  );


  PVDD_08_08_NT_DR_H
  PVDD_08_08_NT_DR_left_cell_7
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PVSS_08_08_NT_DR_H
  PVSS_08_08_NT_DR_left_cell_8
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PDVSS_18_18_NT_DR_H
  PDVSS_18_18_NT_DR_right_cell_9
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PDVDD_18_18_NT_DR_H
  PDVDD_18_18_NT_DR_right_cell_10
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PINCNP_18_18_NT_DR_H
  PINCNP_18_18_NT_DR_right_cell_11
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(prog_clk_pad),
    .Y(prog_clk),
    .IE(TIEHIGH[7]),
    .IS(TIELOW[16]),
    .POE(TIELOW[17]),
    .PO(UNCONN[5])
  );


  PBIDIR_18_18_NT_DR_H
  PBIDIR_18_18_NT_DR_right_cell_12
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[0]),
    .Y(gfpga_pad_GPIO_Y[0]),
    .A(gfpga_pad_GPIO_A[0]),
    .IE(gfpga_pad_GPIO_IE[0]),
    .OE(gfpga_pad_GPIO_OE[0]),
    .DS0(TIEHIGH[8]),
    .DS1(TIEHIGH[9]),
    .IS(TIELOW[18]),
    .PE(TIELOW[19]),
    .POE(TIELOW[20]),
    .PS(TIELOW[21]),
    .SR(TIELOW[22]),
    .PO(UNCONN[6])
  );


  PDVDDTIE_18_18_NT_DR_H
  PDVDDTIE_18_18_NT_DR_right_cell_13
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PBIDIR_18_18_NT_DR_H
  PBIDIR_18_18_NT_DR_right_cell_14
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[1]),
    .Y(gfpga_pad_GPIO_Y[1]),
    .A(gfpga_pad_GPIO_A[1]),
    .IE(gfpga_pad_GPIO_IE[1]),
    .OE(gfpga_pad_GPIO_OE[1]),
    .DS0(TIEHIGH[10]),
    .DS1(TIEHIGH[11]),
    .IS(TIELOW[23]),
    .PE(TIELOW[24]),
    .POE(TIELOW[25]),
    .PS(TIELOW[26]),
    .SR(TIELOW[27]),
    .PO(UNCONN[7])
  );


  PINCNP_18_18_NT_DR_H
  PINCNP_18_18_NT_DR_right_cell_15
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(pReset_pad),
    .Y(pReset),
    .IE(TIEHIGH[12]),
    .IS(TIELOW[28]),
    .POE(TIELOW[29]),
    .PO(UNCONN[8])
  );


  PVDD_08_08_NT_DR_H
  PVDD_08_08_NT_DR_right_cell_16
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PVSS_08_08_NT_DR_H
  PVSS_08_08_NT_DR_right_cell_17
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PDVSS_18_18_NT_DR_V
  PDVSS_18_18_NT_DR_top_cell_18
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PDVDD_18_18_NT_DR_V
  PDVDD_18_18_NT_DR_top_cell_19
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PINCNP_18_18_NT_DR_V
  PINCNP_18_18_NT_DR_top_cell_20
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(sc_head_pad),
    .Y(sc_head),
    .IE(TIEHIGH[13]),
    .IS(TIELOW[30]),
    .POE(TIELOW[31]),
    .PO(UNCONN[9])
  );


  PBIDIR_18_18_NT_DR_V
  PBIDIR_18_18_NT_DR_top_cell_21
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[2]),
    .Y(gfpga_pad_GPIO_Y[2]),
    .A(gfpga_pad_GPIO_A[2]),
    .IE(gfpga_pad_GPIO_IE[2]),
    .OE(gfpga_pad_GPIO_OE[2]),
    .DS0(TIEHIGH[14]),
    .DS1(TIEHIGH[15]),
    .IS(TIELOW[32]),
    .PE(TIELOW[33]),
    .POE(TIELOW[34]),
    .PS(TIELOW[35]),
    .SR(TIELOW[36]),
    .PO(UNCONN[10])
  );


  PDVDDTIE_18_18_NT_DR_V
  PDVDDTIE_18_18_NT_DR_top_cell_22
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PBIDIR_18_18_NT_DR_V
  PBIDIR_18_18_NT_DR_top_cell_23
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[3]),
    .Y(gfpga_pad_GPIO_Y[3]),
    .A(gfpga_pad_GPIO_A[3]),
    .IE(gfpga_pad_GPIO_IE[3]),
    .OE(gfpga_pad_GPIO_OE[3]),
    .DS0(TIEHIGH[16]),
    .DS1(TIEHIGH[17]),
    .IS(TIELOW[37]),
    .PE(TIELOW[38]),
    .POE(TIELOW[39]),
    .PS(TIELOW[40]),
    .SR(TIELOW[41]),
    .PO(UNCONN[11])
  );


  PBIDIR_18_18_NT_DR_V
  PBIDIR_18_18_NT_DR_top_cell_24
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(ccff_tail_pad),
    .A(ccff_tail),
    .Y(UNCONN[12]),
    .IE(TIELOW[42]),
    .OE(TIEHIGH[18]),
    .DS0(TIEHIGH[19]),
    .DS1(TIEHIGH[20]),
    .IS(TIELOW[43]),
    .PE(TIELOW[44]),
    .POE(TIELOW[45]),
    .PS(TIELOW[46]),
    .SR(TIELOW[47]),
    .PO(UNCONN[13])
  );


  PVDD_08_08_NT_DR_V
  PVDD_08_08_NT_DR_top_cell_25
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PVSS_08_08_NT_DR_V
  PVSS_08_08_NT_DR_top_cell_26
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PDVSS_18_18_NT_DR_V
  PDVSS_18_18_NT_DR_bottom_cell_27
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PDVDD_18_18_NT_DR_V
  PDVDD_18_18_NT_DR_bottom_cell_28
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PINCNP_18_18_NT_DR_V
  PINCNP_18_18_NT_DR_bottom_cell_29
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(ccff_head_pad),
    .Y(ccff_head),
    .IE(TIEHIGH[21]),
    .IS(TIELOW[48]),
    .POE(TIELOW[49]),
    .PO(UNCONN[14])
  );


  PBIDIR_18_18_NT_DR_V
  PBIDIR_18_18_NT_DR_bottom_cell_30
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[4]),
    .Y(gfpga_pad_GPIO_Y[4]),
    .A(gfpga_pad_GPIO_A[4]),
    .IE(gfpga_pad_GPIO_IE[4]),
    .OE(gfpga_pad_GPIO_OE[4]),
    .DS0(TIEHIGH[22]),
    .DS1(TIEHIGH[23]),
    .IS(TIELOW[50]),
    .PE(TIELOW[51]),
    .POE(TIELOW[52]),
    .PS(TIELOW[53]),
    .SR(TIELOW[54]),
    .PO(UNCONN[15])
  );


  PDVDDTIE_18_18_NT_DR_V
  PDVDDTIE_18_18_NT_DR_bottom_cell_31
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PBIDIR_18_18_NT_DR_V
  PBIDIR_18_18_NT_DR_bottom_cell_32
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(gpio_pad[5]),
    .Y(gfpga_pad_GPIO_Y[5]),
    .A(gfpga_pad_GPIO_A[5]),
    .IE(gfpga_pad_GPIO_IE[5]),
    .OE(gfpga_pad_GPIO_OE[5]),
    .DS0(TIEHIGH[24]),
    .DS1(TIEHIGH[25]),
    .IS(TIELOW[55]),
    .PE(TIELOW[56]),
    .POE(TIELOW[57]),
    .PS(TIELOW[58]),
    .SR(TIELOW[59]),
    .PO(UNCONN[16])
  );


  PBIDIR_18_18_NT_DR_V
  PBIDIR_18_18_NT_DR_bottom_cell_33
  (
    .SNS(SNS),
    .RTO(RTO),
    // .DVDD(DVDD),
    // .DVSS(DVSS),
    .PAD(sc_tail_pad),
    .A(sc_tail),
    .Y(UNCONN[17]),
    .IE(TIELOW[60]),
    .OE(TIEHIGH[26]),
    .DS0(TIEHIGH[27]),
    .DS1(TIEHIGH[28]),
    .IS(TIELOW[61]),
    .PE(TIELOW[62]),
    .POE(TIELOW[63]),
    .PS(TIELOW[64]),
    .SR(TIELOW[65]),
    .PO(UNCONN[18])
  );


  PVDD_08_08_NT_DR_V
  PVDD_08_08_NT_DR_bottom_cell_34
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  PVSS_08_08_NT_DR_V
  PVSS_08_08_NT_DR_bottom_cell_35
  (
    .SNS(SNS),
    .RTO(RTO)
    // .DVDD(DVDD),
    // .DVSS(DVSS)
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_0
  (
    .Y(TIEHIGH[0])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_1
  (
    .Y(TIEHIGH[1])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_2
  (
    .Y(TIEHIGH[2])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_3
  (
    .Y(TIEHIGH[3])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_4
  (
    .Y(TIEHIGH[4])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_5
  (
    .Y(TIEHIGH[5])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_6
  (
    .Y(TIEHIGH[6])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_7
  (
    .Y(TIEHIGH[7])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_8
  (
    .Y(TIEHIGH[8])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_9
  (
    .Y(TIEHIGH[9])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_10
  (
    .Y(TIEHIGH[10])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_11
  (
    .Y(TIEHIGH[11])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_12
  (
    .Y(TIEHIGH[12])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_13
  (
    .Y(TIEHIGH[13])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_14
  (
    .Y(TIEHIGH[14])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_15
  (
    .Y(TIEHIGH[15])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_16
  (
    .Y(TIEHIGH[16])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_17
  (
    .Y(TIEHIGH[17])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_18
  (
    .Y(TIEHIGH[18])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_19
  (
    .Y(TIEHIGH[19])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_20
  (
    .Y(TIEHIGH[20])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_21
  (
    .Y(TIEHIGH[21])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_22
  (
    .Y(TIEHIGH[22])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_23
  (
    .Y(TIEHIGH[23])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_24
  (
    .Y(TIEHIGH[24])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_25
  (
    .Y(TIEHIGH[25])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_26
  (
    .Y(TIEHIGH[26])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_27
  (
    .Y(TIEHIGH[27])
  );


  TIEHI_X1N_A9PP84TR_C14
  tie_high_28
  (
    .Y(TIEHIGH[28])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_0
  (
    .Y(TIELOW[0])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_1
  (
    .Y(TIELOW[1])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_2
  (
    .Y(TIELOW[2])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_3
  (
    .Y(TIELOW[3])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_4
  (
    .Y(TIELOW[4])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_5
  (
    .Y(TIELOW[5])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_6
  (
    .Y(TIELOW[6])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_7
  (
    .Y(TIELOW[7])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_8
  (
    .Y(TIELOW[8])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_9
  (
    .Y(TIELOW[9])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_10
  (
    .Y(TIELOW[10])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_11
  (
    .Y(TIELOW[11])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_12
  (
    .Y(TIELOW[12])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_13
  (
    .Y(TIELOW[13])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_14
  (
    .Y(TIELOW[14])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_15
  (
    .Y(TIELOW[15])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_16
  (
    .Y(TIELOW[16])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_17
  (
    .Y(TIELOW[17])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_18
  (
    .Y(TIELOW[18])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_19
  (
    .Y(TIELOW[19])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_20
  (
    .Y(TIELOW[20])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_21
  (
    .Y(TIELOW[21])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_22
  (
    .Y(TIELOW[22])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_23
  (
    .Y(TIELOW[23])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_24
  (
    .Y(TIELOW[24])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_25
  (
    .Y(TIELOW[25])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_26
  (
    .Y(TIELOW[26])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_27
  (
    .Y(TIELOW[27])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_28
  (
    .Y(TIELOW[28])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_29
  (
    .Y(TIELOW[29])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_30
  (
    .Y(TIELOW[30])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_31
  (
    .Y(TIELOW[31])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_32
  (
    .Y(TIELOW[32])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_33
  (
    .Y(TIELOW[33])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_34
  (
    .Y(TIELOW[34])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_35
  (
    .Y(TIELOW[35])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_36
  (
    .Y(TIELOW[36])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_37
  (
    .Y(TIELOW[37])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_38
  (
    .Y(TIELOW[38])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_39
  (
    .Y(TIELOW[39])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_40
  (
    .Y(TIELOW[40])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_41
  (
    .Y(TIELOW[41])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_42
  (
    .Y(TIELOW[42])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_43
  (
    .Y(TIELOW[43])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_44
  (
    .Y(TIELOW[44])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_45
  (
    .Y(TIELOW[45])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_46
  (
    .Y(TIELOW[46])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_47
  (
    .Y(TIELOW[47])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_48
  (
    .Y(TIELOW[48])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_49
  (
    .Y(TIELOW[49])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_50
  (
    .Y(TIELOW[50])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_51
  (
    .Y(TIELOW[51])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_52
  (
    .Y(TIELOW[52])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_53
  (
    .Y(TIELOW[53])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_54
  (
    .Y(TIELOW[54])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_55
  (
    .Y(TIELOW[55])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_56
  (
    .Y(TIELOW[56])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_57
  (
    .Y(TIELOW[57])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_58
  (
    .Y(TIELOW[58])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_59
  (
    .Y(TIELOW[59])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_60
  (
    .Y(TIELOW[60])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_61
  (
    .Y(TIELOW[61])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_62
  (
    .Y(TIELOW[62])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_63
  (
    .Y(TIELOW[63])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_64
  (
    .Y(TIELOW[64])
  );


  TIELO_X1N_A9PP84TR_C14
  tie_low_65
  (
    .Y(TIELOW[65])
  );


endmodule

