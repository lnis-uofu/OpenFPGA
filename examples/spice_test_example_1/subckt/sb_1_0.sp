*****************************
*     FPGA SPICE Netlist    *
* Description: Switch Block  [1][0] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Switch Box[1][0] Sub-Circuit *****
.subckt sb[1][0] 
***** Inputs/outputs of top side *****
+ chany[1][1]_out[0] chany[1][1]_in[1] chany[1][1]_out[2] chany[1][1]_in[3] chany[1][1]_out[4] chany[1][1]_in[5] chany[1][1]_out[6] chany[1][1]_in[7] chany[1][1]_out[8] chany[1][1]_in[9] chany[1][1]_out[10] chany[1][1]_in[11] chany[1][1]_out[12] chany[1][1]_in[13] chany[1][1]_out[14] chany[1][1]_in[15] chany[1][1]_out[16] chany[1][1]_in[17] chany[1][1]_out[18] chany[1][1]_in[19] chany[1][1]_out[20] chany[1][1]_in[21] chany[1][1]_out[22] chany[1][1]_in[23] chany[1][1]_out[24] chany[1][1]_in[25] chany[1][1]_out[26] chany[1][1]_in[27] chany[1][1]_out[28] chany[1][1]_in[29] 
+ grid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][3] grid[2][1]_pin[0][3][5] grid[2][1]_pin[0][3][7] grid[2][1]_pin[0][3][9] grid[2][1]_pin[0][3][11] grid[2][1]_pin[0][3][13] grid[2][1]_pin[0][3][15] 
+ ***** Inputs/outputs of right side *****
+ 
+ 
+ ***** Inputs/outputs of bottom side *****
+ 
+ 
+ ***** Inputs/outputs of left side *****
+ chanx[1][0]_in[0] chanx[1][0]_out[1] chanx[1][0]_in[2] chanx[1][0]_out[3] chanx[1][0]_in[4] chanx[1][0]_out[5] chanx[1][0]_in[6] chanx[1][0]_out[7] chanx[1][0]_in[8] chanx[1][0]_out[9] chanx[1][0]_in[10] chanx[1][0]_out[11] chanx[1][0]_in[12] chanx[1][0]_out[13] chanx[1][0]_in[14] chanx[1][0]_out[15] chanx[1][0]_in[16] chanx[1][0]_out[17] chanx[1][0]_in[18] chanx[1][0]_out[19] chanx[1][0]_in[20] chanx[1][0]_out[21] chanx[1][0]_in[22] chanx[1][0]_out[23] chanx[1][0]_in[24] chanx[1][0]_out[25] chanx[1][0]_in[26] chanx[1][0]_out[27] chanx[1][0]_in[28] chanx[1][0]_out[29] 
+ grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][3] grid[1][0]_pin[0][0][5] grid[1][0]_pin[0][0][7] grid[1][0]_pin[0][0][9] grid[1][0]_pin[0][0][11] grid[1][0]_pin[0][0][13] grid[1][0]_pin[0][0][15] 
+ svdd sgnd
***** top side Multiplexers *****
Xmux_1level_tapbuf_size3[61] grid[2][1]_pin[0][3][1] grid[2][1]_pin[0][3][15] chanx[1][0]_in[0] chany[1][1]_out[0] sram[145]->outb sram[145]->out sram[146]->out sram[146]->outb sram[147]->out sram[147]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[61], level=1, select_path_id=0. *****
*****100*****
Xsram[145] sram->in sram[145]->out sram[145]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[145]->out) 0
.nodeset V(sram[145]->outb) vsp
Xsram[146] sram->in sram[146]->out sram[146]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[146]->out) 0
.nodeset V(sram[146]->outb) vsp
Xsram[147] sram->in sram[147]->out sram[147]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[147]->out) 0
.nodeset V(sram[147]->outb) vsp
Xmux_1level_tapbuf_size2[62] grid[2][1]_pin[0][3][1] chanx[1][0]_in[28] chany[1][1]_out[2] sram[148]->outb sram[148]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[62], level=1, select_path_id=0. *****
*****1*****
Xsram[148] sram->in sram[148]->out sram[148]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[148]->out) 0
.nodeset V(sram[148]->outb) vsp
Xmux_1level_tapbuf_size2[63] grid[2][1]_pin[0][3][3] chanx[1][0]_in[26] chany[1][1]_out[4] sram[149]->outb sram[149]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[63], level=1, select_path_id=0. *****
*****1*****
Xsram[149] sram->in sram[149]->out sram[149]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[149]->out) 0
.nodeset V(sram[149]->outb) vsp
Xmux_1level_tapbuf_size2[64] grid[2][1]_pin[0][3][3] chanx[1][0]_in[24] chany[1][1]_out[6] sram[150]->outb sram[150]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[64], level=1, select_path_id=0. *****
*****1*****
Xsram[150] sram->in sram[150]->out sram[150]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[150]->out) 0
.nodeset V(sram[150]->outb) vsp
Xmux_1level_tapbuf_size2[65] grid[2][1]_pin[0][3][5] chanx[1][0]_in[22] chany[1][1]_out[8] sram[151]->outb sram[151]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[65], level=1, select_path_id=0. *****
*****1*****
Xsram[151] sram->in sram[151]->out sram[151]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[151]->out) 0
.nodeset V(sram[151]->outb) vsp
Xmux_1level_tapbuf_size2[66] grid[2][1]_pin[0][3][5] chanx[1][0]_in[20] chany[1][1]_out[10] sram[152]->outb sram[152]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[66], level=1, select_path_id=0. *****
*****1*****
Xsram[152] sram->in sram[152]->out sram[152]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[152]->out) 0
.nodeset V(sram[152]->outb) vsp
Xmux_1level_tapbuf_size2[67] grid[2][1]_pin[0][3][7] chanx[1][0]_in[18] chany[1][1]_out[12] sram[153]->outb sram[153]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[67], level=1, select_path_id=0. *****
*****1*****
Xsram[153] sram->in sram[153]->out sram[153]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[153]->out) 0
.nodeset V(sram[153]->outb) vsp
Xmux_1level_tapbuf_size2[68] grid[2][1]_pin[0][3][7] chanx[1][0]_in[16] chany[1][1]_out[14] sram[154]->outb sram[154]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[68], level=1, select_path_id=0. *****
*****1*****
Xsram[154] sram->in sram[154]->out sram[154]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[154]->out) 0
.nodeset V(sram[154]->outb) vsp
Xmux_1level_tapbuf_size2[69] grid[2][1]_pin[0][3][9] chanx[1][0]_in[14] chany[1][1]_out[16] sram[155]->outb sram[155]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[69], level=1, select_path_id=0. *****
*****1*****
Xsram[155] sram->in sram[155]->out sram[155]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[155]->out) 0
.nodeset V(sram[155]->outb) vsp
Xmux_1level_tapbuf_size2[70] grid[2][1]_pin[0][3][9] chanx[1][0]_in[12] chany[1][1]_out[18] sram[156]->outb sram[156]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[70], level=1, select_path_id=0. *****
*****1*****
Xsram[156] sram->in sram[156]->out sram[156]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[156]->out) 0
.nodeset V(sram[156]->outb) vsp
Xmux_1level_tapbuf_size2[71] grid[2][1]_pin[0][3][11] chanx[1][0]_in[10] chany[1][1]_out[20] sram[157]->outb sram[157]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[71], level=1, select_path_id=0. *****
*****1*****
Xsram[157] sram->in sram[157]->out sram[157]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[157]->out) 0
.nodeset V(sram[157]->outb) vsp
Xmux_1level_tapbuf_size2[72] grid[2][1]_pin[0][3][11] chanx[1][0]_in[8] chany[1][1]_out[22] sram[158]->outb sram[158]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[72], level=1, select_path_id=0. *****
*****1*****
Xsram[158] sram->in sram[158]->out sram[158]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[158]->out) 0
.nodeset V(sram[158]->outb) vsp
Xmux_1level_tapbuf_size2[73] grid[2][1]_pin[0][3][13] chanx[1][0]_in[6] chany[1][1]_out[24] sram[159]->outb sram[159]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[73], level=1, select_path_id=0. *****
*****1*****
Xsram[159] sram->in sram[159]->out sram[159]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[159]->out) 0
.nodeset V(sram[159]->outb) vsp
Xmux_1level_tapbuf_size2[74] grid[2][1]_pin[0][3][13] chanx[1][0]_in[4] chany[1][1]_out[26] sram[160]->outb sram[160]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[74], level=1, select_path_id=0. *****
*****1*****
Xsram[160] sram->in sram[160]->out sram[160]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[160]->out) 0
.nodeset V(sram[160]->outb) vsp
Xmux_1level_tapbuf_size2[75] grid[2][1]_pin[0][3][15] chanx[1][0]_in[2] chany[1][1]_out[28] sram[161]->outb sram[161]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[75], level=1, select_path_id=0. *****
*****1*****
Xsram[161] sram->in sram[161]->out sram[161]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[161]->out) 0
.nodeset V(sram[161]->outb) vsp
***** right side Multiplexers *****
***** bottom side Multiplexers *****
***** left side Multiplexers *****
Xmux_1level_tapbuf_size3[76] grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][15] chany[1][1]_in[1] chanx[1][0]_out[1] sram[162]->outb sram[162]->out sram[163]->out sram[163]->outb sram[164]->out sram[164]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[76], level=1, select_path_id=0. *****
*****100*****
Xsram[162] sram->in sram[162]->out sram[162]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[162]->out) 0
.nodeset V(sram[162]->outb) vsp
Xsram[163] sram->in sram[163]->out sram[163]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[163]->out) 0
.nodeset V(sram[163]->outb) vsp
Xsram[164] sram->in sram[164]->out sram[164]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[164]->out) 0
.nodeset V(sram[164]->outb) vsp
Xmux_1level_tapbuf_size2[77] grid[1][0]_pin[0][0][1] chany[1][1]_in[29] chanx[1][0]_out[3] sram[165]->outb sram[165]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[77], level=1, select_path_id=0. *****
*****1*****
Xsram[165] sram->in sram[165]->out sram[165]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[165]->out) 0
.nodeset V(sram[165]->outb) vsp
Xmux_1level_tapbuf_size2[78] grid[1][0]_pin[0][0][3] chany[1][1]_in[27] chanx[1][0]_out[5] sram[166]->outb sram[166]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[78], level=1, select_path_id=0. *****
*****1*****
Xsram[166] sram->in sram[166]->out sram[166]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[166]->out) 0
.nodeset V(sram[166]->outb) vsp
Xmux_1level_tapbuf_size2[79] grid[1][0]_pin[0][0][3] chany[1][1]_in[25] chanx[1][0]_out[7] sram[167]->outb sram[167]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[79], level=1, select_path_id=0. *****
*****1*****
Xsram[167] sram->in sram[167]->out sram[167]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[167]->out) 0
.nodeset V(sram[167]->outb) vsp
Xmux_1level_tapbuf_size2[80] grid[1][0]_pin[0][0][5] chany[1][1]_in[23] chanx[1][0]_out[9] sram[168]->outb sram[168]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[80], level=1, select_path_id=0. *****
*****1*****
Xsram[168] sram->in sram[168]->out sram[168]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[168]->out) 0
.nodeset V(sram[168]->outb) vsp
Xmux_1level_tapbuf_size2[81] grid[1][0]_pin[0][0][5] chany[1][1]_in[21] chanx[1][0]_out[11] sram[169]->outb sram[169]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[81], level=1, select_path_id=0. *****
*****1*****
Xsram[169] sram->in sram[169]->out sram[169]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[169]->out) 0
.nodeset V(sram[169]->outb) vsp
Xmux_1level_tapbuf_size2[82] grid[1][0]_pin[0][0][7] chany[1][1]_in[19] chanx[1][0]_out[13] sram[170]->outb sram[170]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[82], level=1, select_path_id=0. *****
*****1*****
Xsram[170] sram->in sram[170]->out sram[170]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[170]->out) 0
.nodeset V(sram[170]->outb) vsp
Xmux_1level_tapbuf_size2[83] grid[1][0]_pin[0][0][7] chany[1][1]_in[17] chanx[1][0]_out[15] sram[171]->outb sram[171]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[83], level=1, select_path_id=0. *****
*****1*****
Xsram[171] sram->in sram[171]->out sram[171]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[171]->out) 0
.nodeset V(sram[171]->outb) vsp
Xmux_1level_tapbuf_size2[84] grid[1][0]_pin[0][0][9] chany[1][1]_in[15] chanx[1][0]_out[17] sram[172]->outb sram[172]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[84], level=1, select_path_id=0. *****
*****1*****
Xsram[172] sram->in sram[172]->out sram[172]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[172]->out) 0
.nodeset V(sram[172]->outb) vsp
Xmux_1level_tapbuf_size2[85] grid[1][0]_pin[0][0][9] chany[1][1]_in[13] chanx[1][0]_out[19] sram[173]->outb sram[173]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[85], level=1, select_path_id=0. *****
*****1*****
Xsram[173] sram->in sram[173]->out sram[173]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[173]->out) 0
.nodeset V(sram[173]->outb) vsp
Xmux_1level_tapbuf_size2[86] grid[1][0]_pin[0][0][11] chany[1][1]_in[11] chanx[1][0]_out[21] sram[174]->outb sram[174]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[86], level=1, select_path_id=0. *****
*****1*****
Xsram[174] sram->in sram[174]->out sram[174]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[174]->out) 0
.nodeset V(sram[174]->outb) vsp
Xmux_1level_tapbuf_size2[87] grid[1][0]_pin[0][0][11] chany[1][1]_in[9] chanx[1][0]_out[23] sram[175]->outb sram[175]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[87], level=1, select_path_id=0. *****
*****1*****
Xsram[175] sram->in sram[175]->out sram[175]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[175]->out) 0
.nodeset V(sram[175]->outb) vsp
Xmux_1level_tapbuf_size2[88] grid[1][0]_pin[0][0][13] chany[1][1]_in[7] chanx[1][0]_out[25] sram[176]->outb sram[176]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[88], level=1, select_path_id=0. *****
*****1*****
Xsram[176] sram->in sram[176]->out sram[176]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[176]->out) 0
.nodeset V(sram[176]->outb) vsp
Xmux_1level_tapbuf_size2[89] grid[1][0]_pin[0][0][13] chany[1][1]_in[5] chanx[1][0]_out[27] sram[177]->outb sram[177]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[89], level=1, select_path_id=0. *****
*****1*****
Xsram[177] sram->in sram[177]->out sram[177]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[177]->out) 0
.nodeset V(sram[177]->outb) vsp
Xmux_1level_tapbuf_size2[90] grid[1][0]_pin[0][0][15] chany[1][1]_in[3] chanx[1][0]_out[29] sram[178]->outb sram[178]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[90], level=1, select_path_id=0. *****
*****1*****
Xsram[178] sram->in sram[178]->out sram[178]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[178]->out) 0
.nodeset V(sram[178]->outb) vsp
.eom
