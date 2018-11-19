*****************************
*     FPGA SPICE Netlist    *
* Description: Switch Block  [1][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Switch Box[1][1] Sub-Circuit *****
.subckt sb[1][1] 
***** Inputs/outputs of top side *****
+ 
+ 
+ ***** Inputs/outputs of right side *****
+ 
+ 
+ ***** Inputs/outputs of bottom side *****
+ chany[1][1]_in[0] chany[1][1]_out[1] chany[1][1]_in[2] chany[1][1]_out[3] chany[1][1]_in[4] chany[1][1]_out[5] chany[1][1]_in[6] chany[1][1]_out[7] chany[1][1]_in[8] chany[1][1]_out[9] chany[1][1]_in[10] chany[1][1]_out[11] chany[1][1]_in[12] chany[1][1]_out[13] chany[1][1]_in[14] chany[1][1]_out[15] chany[1][1]_in[16] chany[1][1]_out[17] chany[1][1]_in[18] chany[1][1]_out[19] chany[1][1]_in[20] chany[1][1]_out[21] chany[1][1]_in[22] chany[1][1]_out[23] chany[1][1]_in[24] chany[1][1]_out[25] chany[1][1]_in[26] chany[1][1]_out[27] chany[1][1]_in[28] chany[1][1]_out[29] 
+ grid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][3] grid[2][1]_pin[0][3][5] grid[2][1]_pin[0][3][7] grid[2][1]_pin[0][3][9] grid[2][1]_pin[0][3][11] grid[2][1]_pin[0][3][13] grid[2][1]_pin[0][3][15] 
+ ***** Inputs/outputs of left side *****
+ chanx[1][1]_in[0] chanx[1][1]_out[1] chanx[1][1]_in[2] chanx[1][1]_out[3] chanx[1][1]_in[4] chanx[1][1]_out[5] chanx[1][1]_in[6] chanx[1][1]_out[7] chanx[1][1]_in[8] chanx[1][1]_out[9] chanx[1][1]_in[10] chanx[1][1]_out[11] chanx[1][1]_in[12] chanx[1][1]_out[13] chanx[1][1]_in[14] chanx[1][1]_out[15] chanx[1][1]_in[16] chanx[1][1]_out[17] chanx[1][1]_in[18] chanx[1][1]_out[19] chanx[1][1]_in[20] chanx[1][1]_out[21] chanx[1][1]_in[22] chanx[1][1]_out[23] chanx[1][1]_in[24] chanx[1][1]_out[25] chanx[1][1]_in[26] chanx[1][1]_out[27] chanx[1][1]_in[28] chanx[1][1]_out[29] 
+ grid[1][2]_pin[0][2][1] grid[1][2]_pin[0][2][3] grid[1][2]_pin[0][2][5] grid[1][2]_pin[0][2][7] grid[1][2]_pin[0][2][9] grid[1][2]_pin[0][2][11] grid[1][2]_pin[0][2][13] grid[1][2]_pin[0][2][15] grid[1][1]_pin[0][0][4] 
+ svdd sgnd
***** top side Multiplexers *****
***** right side Multiplexers *****
***** bottom side Multiplexers *****
Xmux_1level_tapbuf_size3[91] grid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][15] chanx[1][1]_in[2] chany[1][1]_out[1] sram[179]->outb sram[179]->out sram[180]->out sram[180]->outb sram[181]->out sram[181]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[91], level=1, select_path_id=0. *****
*****100*****
Xsram[179] sram->in sram[179]->out sram[179]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[179]->out) 0
.nodeset V(sram[179]->outb) vsp
Xsram[180] sram->in sram[180]->out sram[180]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[180]->out) 0
.nodeset V(sram[180]->outb) vsp
Xsram[181] sram->in sram[181]->out sram[181]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[181]->out) 0
.nodeset V(sram[181]->outb) vsp
Xmux_1level_tapbuf_size2[92] grid[2][1]_pin[0][3][1] chanx[1][1]_in[4] chany[1][1]_out[3] sram[182]->outb sram[182]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[92], level=1, select_path_id=0. *****
*****1*****
Xsram[182] sram->in sram[182]->out sram[182]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[182]->out) 0
.nodeset V(sram[182]->outb) vsp
Xmux_1level_tapbuf_size2[93] grid[2][1]_pin[0][3][3] chanx[1][1]_in[6] chany[1][1]_out[5] sram[183]->outb sram[183]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[93], level=1, select_path_id=0. *****
*****1*****
Xsram[183] sram->in sram[183]->out sram[183]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[183]->out) 0
.nodeset V(sram[183]->outb) vsp
Xmux_1level_tapbuf_size2[94] grid[2][1]_pin[0][3][3] chanx[1][1]_in[8] chany[1][1]_out[7] sram[184]->outb sram[184]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[94], level=1, select_path_id=0. *****
*****1*****
Xsram[184] sram->in sram[184]->out sram[184]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[184]->out) 0
.nodeset V(sram[184]->outb) vsp
Xmux_1level_tapbuf_size2[95] grid[2][1]_pin[0][3][5] chanx[1][1]_in[10] chany[1][1]_out[9] sram[185]->outb sram[185]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[95], level=1, select_path_id=0. *****
*****1*****
Xsram[185] sram->in sram[185]->out sram[185]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[185]->out) 0
.nodeset V(sram[185]->outb) vsp
Xmux_1level_tapbuf_size2[96] grid[2][1]_pin[0][3][5] chanx[1][1]_in[12] chany[1][1]_out[11] sram[186]->outb sram[186]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[96], level=1, select_path_id=0. *****
*****1*****
Xsram[186] sram->in sram[186]->out sram[186]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[186]->out) 0
.nodeset V(sram[186]->outb) vsp
Xmux_1level_tapbuf_size2[97] grid[2][1]_pin[0][3][7] chanx[1][1]_in[14] chany[1][1]_out[13] sram[187]->outb sram[187]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[97], level=1, select_path_id=0. *****
*****1*****
Xsram[187] sram->in sram[187]->out sram[187]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[187]->out) 0
.nodeset V(sram[187]->outb) vsp
Xmux_1level_tapbuf_size2[98] grid[2][1]_pin[0][3][7] chanx[1][1]_in[16] chany[1][1]_out[15] sram[188]->outb sram[188]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[98], level=1, select_path_id=0. *****
*****1*****
Xsram[188] sram->in sram[188]->out sram[188]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[188]->out) 0
.nodeset V(sram[188]->outb) vsp
Xmux_1level_tapbuf_size2[99] grid[2][1]_pin[0][3][9] chanx[1][1]_in[18] chany[1][1]_out[17] sram[189]->outb sram[189]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[99], level=1, select_path_id=0. *****
*****1*****
Xsram[189] sram->in sram[189]->out sram[189]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[189]->out) 0
.nodeset V(sram[189]->outb) vsp
Xmux_1level_tapbuf_size2[100] grid[2][1]_pin[0][3][9] chanx[1][1]_in[20] chany[1][1]_out[19] sram[190]->outb sram[190]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[100], level=1, select_path_id=0. *****
*****1*****
Xsram[190] sram->in sram[190]->out sram[190]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[190]->out) 0
.nodeset V(sram[190]->outb) vsp
Xmux_1level_tapbuf_size2[101] grid[2][1]_pin[0][3][11] chanx[1][1]_in[22] chany[1][1]_out[21] sram[191]->outb sram[191]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[101], level=1, select_path_id=0. *****
*****1*****
Xsram[191] sram->in sram[191]->out sram[191]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[191]->out) 0
.nodeset V(sram[191]->outb) vsp
Xmux_1level_tapbuf_size2[102] grid[2][1]_pin[0][3][11] chanx[1][1]_in[24] chany[1][1]_out[23] sram[192]->outb sram[192]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[102], level=1, select_path_id=0. *****
*****1*****
Xsram[192] sram->in sram[192]->out sram[192]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[192]->out) 0
.nodeset V(sram[192]->outb) vsp
Xmux_1level_tapbuf_size2[103] grid[2][1]_pin[0][3][13] chanx[1][1]_in[26] chany[1][1]_out[25] sram[193]->outb sram[193]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[103], level=1, select_path_id=0. *****
*****1*****
Xsram[193] sram->in sram[193]->out sram[193]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[193]->out) 0
.nodeset V(sram[193]->outb) vsp
Xmux_1level_tapbuf_size2[104] grid[2][1]_pin[0][3][13] chanx[1][1]_in[28] chany[1][1]_out[27] sram[194]->outb sram[194]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[104], level=1, select_path_id=0. *****
*****1*****
Xsram[194] sram->in sram[194]->out sram[194]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[194]->out) 0
.nodeset V(sram[194]->outb) vsp
Xmux_1level_tapbuf_size2[105] grid[2][1]_pin[0][3][15] chanx[1][1]_in[0] chany[1][1]_out[29] sram[195]->outb sram[195]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[105], level=1, select_path_id=0. *****
*****1*****
Xsram[195] sram->in sram[195]->out sram[195]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[195]->out) 0
.nodeset V(sram[195]->outb) vsp
***** left side Multiplexers *****
Xmux_1level_tapbuf_size3[106] grid[1][1]_pin[0][0][4] grid[1][2]_pin[0][2][13] chany[1][1]_in[28] chanx[1][1]_out[1] sram[196]->out sram[196]->outb sram[197]->outb sram[197]->out sram[198]->out sram[198]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[106], level=1, select_path_id=1. *****
*****010*****
Xsram[196] sram->in sram[196]->out sram[196]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[196]->out) 0
.nodeset V(sram[196]->outb) vsp
Xsram[197] sram->in sram[197]->out sram[197]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[197]->out) 0
.nodeset V(sram[197]->outb) vsp
Xsram[198] sram->in sram[198]->out sram[198]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[198]->out) 0
.nodeset V(sram[198]->outb) vsp
Xmux_1level_tapbuf_size3[107] grid[1][1]_pin[0][0][4] grid[1][2]_pin[0][2][15] chany[1][1]_in[0] chanx[1][1]_out[3] sram[199]->outb sram[199]->out sram[200]->out sram[200]->outb sram[201]->out sram[201]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[107], level=1, select_path_id=0. *****
*****100*****
Xsram[199] sram->in sram[199]->out sram[199]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[199]->out) 0
.nodeset V(sram[199]->outb) vsp
Xsram[200] sram->in sram[200]->out sram[200]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[200]->out) 0
.nodeset V(sram[200]->outb) vsp
Xsram[201] sram->in sram[201]->out sram[201]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[201]->out) 0
.nodeset V(sram[201]->outb) vsp
Xmux_1level_tapbuf_size3[108] grid[1][2]_pin[0][2][1] grid[1][2]_pin[0][2][15] chany[1][1]_in[2] chanx[1][1]_out[5] sram[202]->outb sram[202]->out sram[203]->out sram[203]->outb sram[204]->out sram[204]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[108], level=1, select_path_id=0. *****
*****100*****
Xsram[202] sram->in sram[202]->out sram[202]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[202]->out) 0
.nodeset V(sram[202]->outb) vsp
Xsram[203] sram->in sram[203]->out sram[203]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[203]->out) 0
.nodeset V(sram[203]->outb) vsp
Xsram[204] sram->in sram[204]->out sram[204]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[204]->out) 0
.nodeset V(sram[204]->outb) vsp
Xmux_1level_tapbuf_size2[109] grid[1][2]_pin[0][2][1] chany[1][1]_in[4] chanx[1][1]_out[7] sram[205]->outb sram[205]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[109], level=1, select_path_id=0. *****
*****1*****
Xsram[205] sram->in sram[205]->out sram[205]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[205]->out) 0
.nodeset V(sram[205]->outb) vsp
Xmux_1level_tapbuf_size2[110] grid[1][2]_pin[0][2][3] chany[1][1]_in[6] chanx[1][1]_out[9] sram[206]->outb sram[206]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[110], level=1, select_path_id=0. *****
*****1*****
Xsram[206] sram->in sram[206]->out sram[206]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[206]->out) 0
.nodeset V(sram[206]->outb) vsp
Xmux_1level_tapbuf_size2[111] grid[1][2]_pin[0][2][3] chany[1][1]_in[8] chanx[1][1]_out[11] sram[207]->outb sram[207]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[111], level=1, select_path_id=0. *****
*****1*****
Xsram[207] sram->in sram[207]->out sram[207]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[207]->out) 0
.nodeset V(sram[207]->outb) vsp
Xmux_1level_tapbuf_size2[112] grid[1][2]_pin[0][2][5] chany[1][1]_in[10] chanx[1][1]_out[13] sram[208]->outb sram[208]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[112], level=1, select_path_id=0. *****
*****1*****
Xsram[208] sram->in sram[208]->out sram[208]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[208]->out) 0
.nodeset V(sram[208]->outb) vsp
Xmux_1level_tapbuf_size2[113] grid[1][2]_pin[0][2][5] chany[1][1]_in[12] chanx[1][1]_out[15] sram[209]->outb sram[209]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[113], level=1, select_path_id=0. *****
*****1*****
Xsram[209] sram->in sram[209]->out sram[209]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[209]->out) 0
.nodeset V(sram[209]->outb) vsp
Xmux_1level_tapbuf_size2[114] grid[1][2]_pin[0][2][7] chany[1][1]_in[14] chanx[1][1]_out[17] sram[210]->outb sram[210]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[114], level=1, select_path_id=0. *****
*****1*****
Xsram[210] sram->in sram[210]->out sram[210]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[210]->out) 0
.nodeset V(sram[210]->outb) vsp
Xmux_1level_tapbuf_size2[115] grid[1][2]_pin[0][2][7] chany[1][1]_in[16] chanx[1][1]_out[19] sram[211]->outb sram[211]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[115], level=1, select_path_id=0. *****
*****1*****
Xsram[211] sram->in sram[211]->out sram[211]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[211]->out) 0
.nodeset V(sram[211]->outb) vsp
Xmux_1level_tapbuf_size2[116] grid[1][2]_pin[0][2][9] chany[1][1]_in[18] chanx[1][1]_out[21] sram[212]->outb sram[212]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[116], level=1, select_path_id=0. *****
*****1*****
Xsram[212] sram->in sram[212]->out sram[212]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[212]->out) 0
.nodeset V(sram[212]->outb) vsp
Xmux_1level_tapbuf_size2[117] grid[1][2]_pin[0][2][9] chany[1][1]_in[20] chanx[1][1]_out[23] sram[213]->outb sram[213]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[117], level=1, select_path_id=0. *****
*****1*****
Xsram[213] sram->in sram[213]->out sram[213]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[213]->out) 0
.nodeset V(sram[213]->outb) vsp
Xmux_1level_tapbuf_size2[118] grid[1][2]_pin[0][2][11] chany[1][1]_in[22] chanx[1][1]_out[25] sram[214]->outb sram[214]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[118], level=1, select_path_id=0. *****
*****1*****
Xsram[214] sram->in sram[214]->out sram[214]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[214]->out) 0
.nodeset V(sram[214]->outb) vsp
Xmux_1level_tapbuf_size2[119] grid[1][2]_pin[0][2][11] chany[1][1]_in[24] chanx[1][1]_out[27] sram[215]->outb sram[215]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[119], level=1, select_path_id=0. *****
*****1*****
Xsram[215] sram->in sram[215]->out sram[215]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[215]->out) 0
.nodeset V(sram[215]->outb) vsp
Xmux_1level_tapbuf_size2[120] grid[1][2]_pin[0][2][13] chany[1][1]_in[26] chanx[1][1]_out[29] sram[216]->outb sram[216]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[120], level=1, select_path_id=0. *****
*****1*****
Xsram[216] sram->in sram[216]->out sram[216]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[216]->out) 0
.nodeset V(sram[216]->outb) vsp
.eom
