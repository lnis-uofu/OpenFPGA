*****************************
*     FPGA SPICE Netlist    *
* Description: Switch Block  [0][0] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Switch Box[0][0] Sub-Circuit *****
.subckt sb[0][0] 
***** Inputs/outputs of top side *****
+ chany[0][1]_out[0] chany[0][1]_in[1] chany[0][1]_out[2] chany[0][1]_in[3] chany[0][1]_out[4] chany[0][1]_in[5] chany[0][1]_out[6] chany[0][1]_in[7] chany[0][1]_out[8] chany[0][1]_in[9] chany[0][1]_out[10] chany[0][1]_in[11] chany[0][1]_out[12] chany[0][1]_in[13] chany[0][1]_out[14] chany[0][1]_in[15] chany[0][1]_out[16] chany[0][1]_in[17] chany[0][1]_out[18] chany[0][1]_in[19] chany[0][1]_out[20] chany[0][1]_in[21] chany[0][1]_out[22] chany[0][1]_in[23] chany[0][1]_out[24] chany[0][1]_in[25] chany[0][1]_out[26] chany[0][1]_in[27] chany[0][1]_out[28] chany[0][1]_in[29] 
+ grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][3] grid[0][1]_pin[0][1][5] grid[0][1]_pin[0][1][7] grid[0][1]_pin[0][1][9] grid[0][1]_pin[0][1][11] grid[0][1]_pin[0][1][13] grid[0][1]_pin[0][1][15] 
+ ***** Inputs/outputs of right side *****
+ chanx[1][0]_out[0] chanx[1][0]_in[1] chanx[1][0]_out[2] chanx[1][0]_in[3] chanx[1][0]_out[4] chanx[1][0]_in[5] chanx[1][0]_out[6] chanx[1][0]_in[7] chanx[1][0]_out[8] chanx[1][0]_in[9] chanx[1][0]_out[10] chanx[1][0]_in[11] chanx[1][0]_out[12] chanx[1][0]_in[13] chanx[1][0]_out[14] chanx[1][0]_in[15] chanx[1][0]_out[16] chanx[1][0]_in[17] chanx[1][0]_out[18] chanx[1][0]_in[19] chanx[1][0]_out[20] chanx[1][0]_in[21] chanx[1][0]_out[22] chanx[1][0]_in[23] chanx[1][0]_out[24] chanx[1][0]_in[25] chanx[1][0]_out[26] chanx[1][0]_in[27] chanx[1][0]_out[28] chanx[1][0]_in[29] 
+ grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][3] grid[1][0]_pin[0][0][5] grid[1][0]_pin[0][0][7] grid[1][0]_pin[0][0][9] grid[1][0]_pin[0][0][11] grid[1][0]_pin[0][0][13] grid[1][0]_pin[0][0][15] 
+ ***** Inputs/outputs of bottom side *****
+ 
+ 
+ ***** Inputs/outputs of left side *****
+ 
+ 
+ svdd sgnd
***** top side Multiplexers *****
Xmux_1level_tapbuf_size3[1] grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][15] chanx[1][0]_in[3] chany[0][1]_out[0] sram[73]->outb sram[73]->out sram[74]->out sram[74]->outb sram[75]->out sram[75]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[1], level=1, select_path_id=0. *****
*****100*****
Xsram[73] sram->in sram[73]->out sram[73]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[73]->out) 0
.nodeset V(sram[73]->outb) vsp
Xsram[74] sram->in sram[74]->out sram[74]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[74]->out) 0
.nodeset V(sram[74]->outb) vsp
Xsram[75] sram->in sram[75]->out sram[75]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[75]->out) 0
.nodeset V(sram[75]->outb) vsp
Xmux_1level_tapbuf_size2[2] grid[0][1]_pin[0][1][1] chanx[1][0]_in[5] chany[0][1]_out[2] sram[76]->outb sram[76]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[2], level=1, select_path_id=0. *****
*****1*****
Xsram[76] sram->in sram[76]->out sram[76]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[76]->out) 0
.nodeset V(sram[76]->outb) vsp
Xmux_1level_tapbuf_size2[3] grid[0][1]_pin[0][1][3] chanx[1][0]_in[7] chany[0][1]_out[4] sram[77]->outb sram[77]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[3], level=1, select_path_id=0. *****
*****1*****
Xsram[77] sram->in sram[77]->out sram[77]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[77]->out) 0
.nodeset V(sram[77]->outb) vsp
Xmux_1level_tapbuf_size2[4] grid[0][1]_pin[0][1][3] chanx[1][0]_in[9] chany[0][1]_out[6] sram[78]->outb sram[78]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[4], level=1, select_path_id=0. *****
*****1*****
Xsram[78] sram->in sram[78]->out sram[78]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[78]->out) 0
.nodeset V(sram[78]->outb) vsp
Xmux_1level_tapbuf_size2[5] grid[0][1]_pin[0][1][5] chanx[1][0]_in[11] chany[0][1]_out[8] sram[79]->outb sram[79]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[5], level=1, select_path_id=0. *****
*****1*****
Xsram[79] sram->in sram[79]->out sram[79]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[79]->out) 0
.nodeset V(sram[79]->outb) vsp
Xmux_1level_tapbuf_size2[6] grid[0][1]_pin[0][1][5] chanx[1][0]_in[13] chany[0][1]_out[10] sram[80]->outb sram[80]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[6], level=1, select_path_id=0. *****
*****1*****
Xsram[80] sram->in sram[80]->out sram[80]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[80]->out) 0
.nodeset V(sram[80]->outb) vsp
Xmux_1level_tapbuf_size2[7] grid[0][1]_pin[0][1][7] chanx[1][0]_in[15] chany[0][1]_out[12] sram[81]->outb sram[81]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[7], level=1, select_path_id=0. *****
*****1*****
Xsram[81] sram->in sram[81]->out sram[81]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[81]->out) 0
.nodeset V(sram[81]->outb) vsp
Xmux_1level_tapbuf_size2[8] grid[0][1]_pin[0][1][7] chanx[1][0]_in[17] chany[0][1]_out[14] sram[82]->outb sram[82]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[8], level=1, select_path_id=0. *****
*****1*****
Xsram[82] sram->in sram[82]->out sram[82]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[82]->out) 0
.nodeset V(sram[82]->outb) vsp
Xmux_1level_tapbuf_size2[9] grid[0][1]_pin[0][1][9] chanx[1][0]_in[19] chany[0][1]_out[16] sram[83]->outb sram[83]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[9], level=1, select_path_id=0. *****
*****1*****
Xsram[83] sram->in sram[83]->out sram[83]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[83]->out) 0
.nodeset V(sram[83]->outb) vsp
Xmux_1level_tapbuf_size2[10] grid[0][1]_pin[0][1][9] chanx[1][0]_in[21] chany[0][1]_out[18] sram[84]->outb sram[84]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[10], level=1, select_path_id=0. *****
*****1*****
Xsram[84] sram->in sram[84]->out sram[84]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[84]->out) 0
.nodeset V(sram[84]->outb) vsp
Xmux_1level_tapbuf_size2[11] grid[0][1]_pin[0][1][11] chanx[1][0]_in[23] chany[0][1]_out[20] sram[85]->outb sram[85]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[11], level=1, select_path_id=0. *****
*****1*****
Xsram[85] sram->in sram[85]->out sram[85]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[85]->out) 0
.nodeset V(sram[85]->outb) vsp
Xmux_1level_tapbuf_size2[12] grid[0][1]_pin[0][1][11] chanx[1][0]_in[25] chany[0][1]_out[22] sram[86]->outb sram[86]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[12], level=1, select_path_id=0. *****
*****1*****
Xsram[86] sram->in sram[86]->out sram[86]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[86]->out) 0
.nodeset V(sram[86]->outb) vsp
Xmux_1level_tapbuf_size2[13] grid[0][1]_pin[0][1][13] chanx[1][0]_in[27] chany[0][1]_out[24] sram[87]->outb sram[87]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[13], level=1, select_path_id=0. *****
*****1*****
Xsram[87] sram->in sram[87]->out sram[87]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[87]->out) 0
.nodeset V(sram[87]->outb) vsp
Xmux_1level_tapbuf_size2[14] grid[0][1]_pin[0][1][13] chanx[1][0]_in[29] chany[0][1]_out[26] sram[88]->outb sram[88]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[14], level=1, select_path_id=0. *****
*****1*****
Xsram[88] sram->in sram[88]->out sram[88]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[88]->out) 0
.nodeset V(sram[88]->outb) vsp
Xmux_1level_tapbuf_size2[15] grid[0][1]_pin[0][1][15] chanx[1][0]_in[1] chany[0][1]_out[28] sram[89]->outb sram[89]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[15], level=1, select_path_id=0. *****
*****1*****
Xsram[89] sram->in sram[89]->out sram[89]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[89]->out) 0
.nodeset V(sram[89]->outb) vsp
***** right side Multiplexers *****
Xmux_1level_tapbuf_size3[16] grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][15] chany[0][1]_in[29] chanx[1][0]_out[0] sram[90]->outb sram[90]->out sram[91]->out sram[91]->outb sram[92]->out sram[92]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[16], level=1, select_path_id=0. *****
*****100*****
Xsram[90] sram->in sram[90]->out sram[90]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[90]->out) 0
.nodeset V(sram[90]->outb) vsp
Xsram[91] sram->in sram[91]->out sram[91]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[91]->out) 0
.nodeset V(sram[91]->outb) vsp
Xsram[92] sram->in sram[92]->out sram[92]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[92]->out) 0
.nodeset V(sram[92]->outb) vsp
Xmux_1level_tapbuf_size2[17] grid[1][0]_pin[0][0][1] chany[0][1]_in[1] chanx[1][0]_out[2] sram[93]->outb sram[93]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[17], level=1, select_path_id=0. *****
*****1*****
Xsram[93] sram->in sram[93]->out sram[93]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[93]->out) 0
.nodeset V(sram[93]->outb) vsp
Xmux_1level_tapbuf_size2[18] grid[1][0]_pin[0][0][3] chany[0][1]_in[3] chanx[1][0]_out[4] sram[94]->outb sram[94]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[18], level=1, select_path_id=0. *****
*****1*****
Xsram[94] sram->in sram[94]->out sram[94]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[94]->out) 0
.nodeset V(sram[94]->outb) vsp
Xmux_1level_tapbuf_size2[19] grid[1][0]_pin[0][0][3] chany[0][1]_in[5] chanx[1][0]_out[6] sram[95]->outb sram[95]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[19], level=1, select_path_id=0. *****
*****1*****
Xsram[95] sram->in sram[95]->out sram[95]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[95]->out) 0
.nodeset V(sram[95]->outb) vsp
Xmux_1level_tapbuf_size2[20] grid[1][0]_pin[0][0][5] chany[0][1]_in[7] chanx[1][0]_out[8] sram[96]->outb sram[96]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[20], level=1, select_path_id=0. *****
*****1*****
Xsram[96] sram->in sram[96]->out sram[96]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[96]->out) 0
.nodeset V(sram[96]->outb) vsp
Xmux_1level_tapbuf_size2[21] grid[1][0]_pin[0][0][5] chany[0][1]_in[9] chanx[1][0]_out[10] sram[97]->outb sram[97]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[21], level=1, select_path_id=0. *****
*****1*****
Xsram[97] sram->in sram[97]->out sram[97]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[97]->out) 0
.nodeset V(sram[97]->outb) vsp
Xmux_1level_tapbuf_size2[22] grid[1][0]_pin[0][0][7] chany[0][1]_in[11] chanx[1][0]_out[12] sram[98]->outb sram[98]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[22], level=1, select_path_id=0. *****
*****1*****
Xsram[98] sram->in sram[98]->out sram[98]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[98]->out) 0
.nodeset V(sram[98]->outb) vsp
Xmux_1level_tapbuf_size2[23] grid[1][0]_pin[0][0][7] chany[0][1]_in[13] chanx[1][0]_out[14] sram[99]->outb sram[99]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[23], level=1, select_path_id=0. *****
*****1*****
Xsram[99] sram->in sram[99]->out sram[99]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[99]->out) 0
.nodeset V(sram[99]->outb) vsp
Xmux_1level_tapbuf_size2[24] grid[1][0]_pin[0][0][9] chany[0][1]_in[15] chanx[1][0]_out[16] sram[100]->outb sram[100]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[24], level=1, select_path_id=0. *****
*****1*****
Xsram[100] sram->in sram[100]->out sram[100]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[100]->out) 0
.nodeset V(sram[100]->outb) vsp
Xmux_1level_tapbuf_size2[25] grid[1][0]_pin[0][0][9] chany[0][1]_in[17] chanx[1][0]_out[18] sram[101]->outb sram[101]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[25], level=1, select_path_id=0. *****
*****1*****
Xsram[101] sram->in sram[101]->out sram[101]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[101]->out) 0
.nodeset V(sram[101]->outb) vsp
Xmux_1level_tapbuf_size2[26] grid[1][0]_pin[0][0][11] chany[0][1]_in[19] chanx[1][0]_out[20] sram[102]->outb sram[102]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[26], level=1, select_path_id=0. *****
*****1*****
Xsram[102] sram->in sram[102]->out sram[102]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[102]->out) 0
.nodeset V(sram[102]->outb) vsp
Xmux_1level_tapbuf_size2[27] grid[1][0]_pin[0][0][11] chany[0][1]_in[21] chanx[1][0]_out[22] sram[103]->outb sram[103]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[27], level=1, select_path_id=0. *****
*****1*****
Xsram[103] sram->in sram[103]->out sram[103]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[103]->out) 0
.nodeset V(sram[103]->outb) vsp
Xmux_1level_tapbuf_size2[28] grid[1][0]_pin[0][0][13] chany[0][1]_in[23] chanx[1][0]_out[24] sram[104]->outb sram[104]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[28], level=1, select_path_id=0. *****
*****1*****
Xsram[104] sram->in sram[104]->out sram[104]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[104]->out) 0
.nodeset V(sram[104]->outb) vsp
Xmux_1level_tapbuf_size2[29] grid[1][0]_pin[0][0][13] chany[0][1]_in[25] chanx[1][0]_out[26] sram[105]->outb sram[105]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[29], level=1, select_path_id=0. *****
*****1*****
Xsram[105] sram->in sram[105]->out sram[105]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[105]->out) 0
.nodeset V(sram[105]->outb) vsp
Xmux_1level_tapbuf_size2[30] grid[1][0]_pin[0][0][15] chany[0][1]_in[27] chanx[1][0]_out[28] sram[106]->outb sram[106]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[30], level=1, select_path_id=0. *****
*****1*****
Xsram[106] sram->in sram[106]->out sram[106]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[106]->out) 0
.nodeset V(sram[106]->outb) vsp
***** bottom side Multiplexers *****
***** left side Multiplexers *****
.eom
