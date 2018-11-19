*****************************
*     FPGA SPICE Netlist    *
* Description: Switch Block  [0][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Switch Box[0][1] Sub-Circuit *****
.subckt sb[0][1] 
***** Inputs/outputs of top side *****
+ 
+ 
+ ***** Inputs/outputs of right side *****
+ chanx[1][1]_out[0] chanx[1][1]_in[1] chanx[1][1]_out[2] chanx[1][1]_in[3] chanx[1][1]_out[4] chanx[1][1]_in[5] chanx[1][1]_out[6] chanx[1][1]_in[7] chanx[1][1]_out[8] chanx[1][1]_in[9] chanx[1][1]_out[10] chanx[1][1]_in[11] chanx[1][1]_out[12] chanx[1][1]_in[13] chanx[1][1]_out[14] chanx[1][1]_in[15] chanx[1][1]_out[16] chanx[1][1]_in[17] chanx[1][1]_out[18] chanx[1][1]_in[19] chanx[1][1]_out[20] chanx[1][1]_in[21] chanx[1][1]_out[22] chanx[1][1]_in[23] chanx[1][1]_out[24] chanx[1][1]_in[25] chanx[1][1]_out[26] chanx[1][1]_in[27] chanx[1][1]_out[28] chanx[1][1]_in[29] 
+ grid[1][2]_pin[0][2][1] grid[1][2]_pin[0][2][3] grid[1][2]_pin[0][2][5] grid[1][2]_pin[0][2][7] grid[1][2]_pin[0][2][9] grid[1][2]_pin[0][2][11] grid[1][2]_pin[0][2][13] grid[1][2]_pin[0][2][15] grid[1][1]_pin[0][0][4] 
+ ***** Inputs/outputs of bottom side *****
+ chany[0][1]_in[0] chany[0][1]_out[1] chany[0][1]_in[2] chany[0][1]_out[3] chany[0][1]_in[4] chany[0][1]_out[5] chany[0][1]_in[6] chany[0][1]_out[7] chany[0][1]_in[8] chany[0][1]_out[9] chany[0][1]_in[10] chany[0][1]_out[11] chany[0][1]_in[12] chany[0][1]_out[13] chany[0][1]_in[14] chany[0][1]_out[15] chany[0][1]_in[16] chany[0][1]_out[17] chany[0][1]_in[18] chany[0][1]_out[19] chany[0][1]_in[20] chany[0][1]_out[21] chany[0][1]_in[22] chany[0][1]_out[23] chany[0][1]_in[24] chany[0][1]_out[25] chany[0][1]_in[26] chany[0][1]_out[27] chany[0][1]_in[28] chany[0][1]_out[29] 
+ grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][3] grid[0][1]_pin[0][1][5] grid[0][1]_pin[0][1][7] grid[0][1]_pin[0][1][9] grid[0][1]_pin[0][1][11] grid[0][1]_pin[0][1][13] grid[0][1]_pin[0][1][15] 
+ ***** Inputs/outputs of left side *****
+ 
+ 
+ svdd sgnd
***** top side Multiplexers *****
***** right side Multiplexers *****
Xmux_1level_tapbuf_size3[31] grid[1][1]_pin[0][0][4] grid[1][2]_pin[0][2][13] chany[0][1]_in[26] chanx[1][1]_out[0] sram[107]->outb sram[107]->out sram[108]->out sram[108]->outb sram[109]->out sram[109]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[31], level=1, select_path_id=0. *****
*****100*****
Xsram[107] sram->in sram[107]->out sram[107]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[107]->out) 0
.nodeset V(sram[107]->outb) vsp
Xsram[108] sram->in sram[108]->out sram[108]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[108]->out) 0
.nodeset V(sram[108]->outb) vsp
Xsram[109] sram->in sram[109]->out sram[109]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[109]->out) 0
.nodeset V(sram[109]->outb) vsp
Xmux_1level_tapbuf_size3[32] grid[1][1]_pin[0][0][4] grid[1][2]_pin[0][2][15] chany[0][1]_in[24] chanx[1][1]_out[2] sram[110]->outb sram[110]->out sram[111]->out sram[111]->outb sram[112]->out sram[112]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[32], level=1, select_path_id=0. *****
*****100*****
Xsram[110] sram->in sram[110]->out sram[110]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[110]->out) 0
.nodeset V(sram[110]->outb) vsp
Xsram[111] sram->in sram[111]->out sram[111]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[111]->out) 0
.nodeset V(sram[111]->outb) vsp
Xsram[112] sram->in sram[112]->out sram[112]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[112]->out) 0
.nodeset V(sram[112]->outb) vsp
Xmux_1level_tapbuf_size3[33] grid[1][2]_pin[0][2][1] grid[1][2]_pin[0][2][15] chany[0][1]_in[22] chanx[1][1]_out[4] sram[113]->outb sram[113]->out sram[114]->out sram[114]->outb sram[115]->out sram[115]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[33], level=1, select_path_id=0. *****
*****100*****
Xsram[113] sram->in sram[113]->out sram[113]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[113]->out) 0
.nodeset V(sram[113]->outb) vsp
Xsram[114] sram->in sram[114]->out sram[114]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[114]->out) 0
.nodeset V(sram[114]->outb) vsp
Xsram[115] sram->in sram[115]->out sram[115]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[115]->out) 0
.nodeset V(sram[115]->outb) vsp
Xmux_1level_tapbuf_size2[34] grid[1][2]_pin[0][2][1] chany[0][1]_in[20] chanx[1][1]_out[6] sram[116]->outb sram[116]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[34], level=1, select_path_id=0. *****
*****1*****
Xsram[116] sram->in sram[116]->out sram[116]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[116]->out) 0
.nodeset V(sram[116]->outb) vsp
Xmux_1level_tapbuf_size2[35] grid[1][2]_pin[0][2][3] chany[0][1]_in[18] chanx[1][1]_out[8] sram[117]->outb sram[117]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[35], level=1, select_path_id=0. *****
*****1*****
Xsram[117] sram->in sram[117]->out sram[117]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[117]->out) 0
.nodeset V(sram[117]->outb) vsp
Xmux_1level_tapbuf_size2[36] grid[1][2]_pin[0][2][3] chany[0][1]_in[16] chanx[1][1]_out[10] sram[118]->outb sram[118]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[36], level=1, select_path_id=0. *****
*****1*****
Xsram[118] sram->in sram[118]->out sram[118]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[118]->out) 0
.nodeset V(sram[118]->outb) vsp
Xmux_1level_tapbuf_size2[37] grid[1][2]_pin[0][2][5] chany[0][1]_in[14] chanx[1][1]_out[12] sram[119]->outb sram[119]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[37], level=1, select_path_id=0. *****
*****1*****
Xsram[119] sram->in sram[119]->out sram[119]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[119]->out) 0
.nodeset V(sram[119]->outb) vsp
Xmux_1level_tapbuf_size2[38] grid[1][2]_pin[0][2][5] chany[0][1]_in[12] chanx[1][1]_out[14] sram[120]->outb sram[120]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[38], level=1, select_path_id=0. *****
*****1*****
Xsram[120] sram->in sram[120]->out sram[120]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[120]->out) 0
.nodeset V(sram[120]->outb) vsp
Xmux_1level_tapbuf_size2[39] grid[1][2]_pin[0][2][7] chany[0][1]_in[10] chanx[1][1]_out[16] sram[121]->outb sram[121]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[39], level=1, select_path_id=0. *****
*****1*****
Xsram[121] sram->in sram[121]->out sram[121]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[121]->out) 0
.nodeset V(sram[121]->outb) vsp
Xmux_1level_tapbuf_size2[40] grid[1][2]_pin[0][2][7] chany[0][1]_in[8] chanx[1][1]_out[18] sram[122]->outb sram[122]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[40], level=1, select_path_id=0. *****
*****1*****
Xsram[122] sram->in sram[122]->out sram[122]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[122]->out) 0
.nodeset V(sram[122]->outb) vsp
Xmux_1level_tapbuf_size2[41] grid[1][2]_pin[0][2][9] chany[0][1]_in[6] chanx[1][1]_out[20] sram[123]->outb sram[123]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[41], level=1, select_path_id=0. *****
*****1*****
Xsram[123] sram->in sram[123]->out sram[123]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[123]->out) 0
.nodeset V(sram[123]->outb) vsp
Xmux_1level_tapbuf_size2[42] grid[1][2]_pin[0][2][9] chany[0][1]_in[4] chanx[1][1]_out[22] sram[124]->outb sram[124]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[42], level=1, select_path_id=0. *****
*****1*****
Xsram[124] sram->in sram[124]->out sram[124]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[124]->out) 0
.nodeset V(sram[124]->outb) vsp
Xmux_1level_tapbuf_size2[43] grid[1][2]_pin[0][2][11] chany[0][1]_in[2] chanx[1][1]_out[24] sram[125]->outb sram[125]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[43], level=1, select_path_id=0. *****
*****1*****
Xsram[125] sram->in sram[125]->out sram[125]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[125]->out) 0
.nodeset V(sram[125]->outb) vsp
Xmux_1level_tapbuf_size2[44] grid[1][2]_pin[0][2][11] chany[0][1]_in[0] chanx[1][1]_out[26] sram[126]->outb sram[126]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[44], level=1, select_path_id=0. *****
*****1*****
Xsram[126] sram->in sram[126]->out sram[126]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[126]->out) 0
.nodeset V(sram[126]->outb) vsp
Xmux_1level_tapbuf_size2[45] grid[1][2]_pin[0][2][13] chany[0][1]_in[28] chanx[1][1]_out[28] sram[127]->outb sram[127]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[45], level=1, select_path_id=0. *****
*****1*****
Xsram[127] sram->in sram[127]->out sram[127]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[127]->out) 0
.nodeset V(sram[127]->outb) vsp
***** bottom side Multiplexers *****
Xmux_1level_tapbuf_size3[46] grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][15] chanx[1][1]_in[27] chany[0][1]_out[1] sram[128]->outb sram[128]->out sram[129]->out sram[129]->outb sram[130]->out sram[130]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[46], level=1, select_path_id=0. *****
*****100*****
Xsram[128] sram->in sram[128]->out sram[128]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[128]->out) 0
.nodeset V(sram[128]->outb) vsp
Xsram[129] sram->in sram[129]->out sram[129]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[129]->out) 0
.nodeset V(sram[129]->outb) vsp
Xsram[130] sram->in sram[130]->out sram[130]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[130]->out) 0
.nodeset V(sram[130]->outb) vsp
Xmux_1level_tapbuf_size2[47] grid[0][1]_pin[0][1][1] chanx[1][1]_in[25] chany[0][1]_out[3] sram[131]->outb sram[131]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[47], level=1, select_path_id=0. *****
*****1*****
Xsram[131] sram->in sram[131]->out sram[131]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[131]->out) 0
.nodeset V(sram[131]->outb) vsp
Xmux_1level_tapbuf_size2[48] grid[0][1]_pin[0][1][3] chanx[1][1]_in[23] chany[0][1]_out[5] sram[132]->outb sram[132]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[48], level=1, select_path_id=0. *****
*****1*****
Xsram[132] sram->in sram[132]->out sram[132]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[132]->out) 0
.nodeset V(sram[132]->outb) vsp
Xmux_1level_tapbuf_size2[49] grid[0][1]_pin[0][1][3] chanx[1][1]_in[21] chany[0][1]_out[7] sram[133]->outb sram[133]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[49], level=1, select_path_id=0. *****
*****1*****
Xsram[133] sram->in sram[133]->out sram[133]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[133]->out) 0
.nodeset V(sram[133]->outb) vsp
Xmux_1level_tapbuf_size2[50] grid[0][1]_pin[0][1][5] chanx[1][1]_in[19] chany[0][1]_out[9] sram[134]->outb sram[134]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[50], level=1, select_path_id=0. *****
*****1*****
Xsram[134] sram->in sram[134]->out sram[134]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[134]->out) 0
.nodeset V(sram[134]->outb) vsp
Xmux_1level_tapbuf_size2[51] grid[0][1]_pin[0][1][5] chanx[1][1]_in[17] chany[0][1]_out[11] sram[135]->outb sram[135]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[51], level=1, select_path_id=0. *****
*****1*****
Xsram[135] sram->in sram[135]->out sram[135]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[135]->out) 0
.nodeset V(sram[135]->outb) vsp
Xmux_1level_tapbuf_size2[52] grid[0][1]_pin[0][1][7] chanx[1][1]_in[15] chany[0][1]_out[13] sram[136]->outb sram[136]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[52], level=1, select_path_id=0. *****
*****1*****
Xsram[136] sram->in sram[136]->out sram[136]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[136]->out) 0
.nodeset V(sram[136]->outb) vsp
Xmux_1level_tapbuf_size2[53] grid[0][1]_pin[0][1][7] chanx[1][1]_in[13] chany[0][1]_out[15] sram[137]->outb sram[137]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[53], level=1, select_path_id=0. *****
*****1*****
Xsram[137] sram->in sram[137]->out sram[137]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[137]->out) 0
.nodeset V(sram[137]->outb) vsp
Xmux_1level_tapbuf_size2[54] grid[0][1]_pin[0][1][9] chanx[1][1]_in[11] chany[0][1]_out[17] sram[138]->outb sram[138]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[54], level=1, select_path_id=0. *****
*****1*****
Xsram[138] sram->in sram[138]->out sram[138]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[138]->out) 0
.nodeset V(sram[138]->outb) vsp
Xmux_1level_tapbuf_size2[55] grid[0][1]_pin[0][1][9] chanx[1][1]_in[9] chany[0][1]_out[19] sram[139]->outb sram[139]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[55], level=1, select_path_id=0. *****
*****1*****
Xsram[139] sram->in sram[139]->out sram[139]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[139]->out) 0
.nodeset V(sram[139]->outb) vsp
Xmux_1level_tapbuf_size2[56] grid[0][1]_pin[0][1][11] chanx[1][1]_in[7] chany[0][1]_out[21] sram[140]->outb sram[140]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[56], level=1, select_path_id=0. *****
*****1*****
Xsram[140] sram->in sram[140]->out sram[140]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[140]->out) 0
.nodeset V(sram[140]->outb) vsp
Xmux_1level_tapbuf_size2[57] grid[0][1]_pin[0][1][11] chanx[1][1]_in[5] chany[0][1]_out[23] sram[141]->outb sram[141]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[57], level=1, select_path_id=0. *****
*****1*****
Xsram[141] sram->in sram[141]->out sram[141]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[141]->out) 0
.nodeset V(sram[141]->outb) vsp
Xmux_1level_tapbuf_size2[58] grid[0][1]_pin[0][1][13] chanx[1][1]_in[3] chany[0][1]_out[25] sram[142]->outb sram[142]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[58], level=1, select_path_id=0. *****
*****1*****
Xsram[142] sram->in sram[142]->out sram[142]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[142]->out) 0
.nodeset V(sram[142]->outb) vsp
Xmux_1level_tapbuf_size2[59] grid[0][1]_pin[0][1][13] chanx[1][1]_in[1] chany[0][1]_out[27] sram[143]->out sram[143]->outb svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[59], level=1, select_path_id=1. *****
*****0*****
Xsram[143] sram->in sram[143]->out sram[143]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[143]->out) 0
.nodeset V(sram[143]->outb) vsp
Xmux_1level_tapbuf_size2[60] grid[0][1]_pin[0][1][15] chanx[1][1]_in[29] chany[0][1]_out[29] sram[144]->outb sram[144]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[60], level=1, select_path_id=0. *****
*****1*****
Xsram[144] sram->in sram[144]->out sram[144]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[144]->out) 0
.nodeset V(sram[144]->outb) vsp
***** left side Multiplexers *****
.eom
