*****************************
*     FPGA SPICE Netlist    *
* Description: Connection Block Y-channel  [1][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
.subckt cby[1][1] 
+ chany[1][1]_midout[0] 
+ chany[1][1]_midout[1] 
+ chany[1][1]_midout[2] 
+ chany[1][1]_midout[3] 
+ chany[1][1]_midout[4] 
+ chany[1][1]_midout[5] 
+ chany[1][1]_midout[6] 
+ chany[1][1]_midout[7] 
+ chany[1][1]_midout[8] 
+ chany[1][1]_midout[9] 
+ chany[1][1]_midout[10] 
+ chany[1][1]_midout[11] 
+ chany[1][1]_midout[12] 
+ chany[1][1]_midout[13] 
+ chany[1][1]_midout[14] 
+ chany[1][1]_midout[15] 
+ chany[1][1]_midout[16] 
+ chany[1][1]_midout[17] 
+ chany[1][1]_midout[18] 
+ chany[1][1]_midout[19] 
+ chany[1][1]_midout[20] 
+ chany[1][1]_midout[21] 
+ chany[1][1]_midout[22] 
+ chany[1][1]_midout[23] 
+ chany[1][1]_midout[24] 
+ chany[1][1]_midout[25] 
+ chany[1][1]_midout[26] 
+ chany[1][1]_midout[27] 
+ chany[1][1]_midout[28] 
+ chany[1][1]_midout[29] 
+ grid[2][1]_pin[0][3][0] 
+ grid[2][1]_pin[0][3][2] 
+ grid[2][1]_pin[0][3][4] 
+ grid[2][1]_pin[0][3][6] 
+ grid[2][1]_pin[0][3][8] 
+ grid[2][1]_pin[0][3][10] 
+ grid[2][1]_pin[0][3][12] 
+ grid[2][1]_pin[0][3][14] 
+ grid[1][1]_pin[0][1][1] 
+ grid[1][1]_pin[0][1][5] 
+ svdd sgnd
Xmux_2level_tapbuf_size4[27] chany[1][1]_midout[6] chany[1][1]_midout[7] chany[1][1]_midout[18] chany[1][1]_midout[19] grid[2][1]_pin[0][3][0] sram[325]->outb sram[325]->out sram[326]->out sram[326]->outb sram[327]->outb sram[327]->out sram[328]->out sram[328]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[27], level=2, select_path_id=0. *****
*****1010*****
Xsram[325] sram->in sram[325]->out sram[325]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[325]->out) 0
.nodeset V(sram[325]->outb) vsp
Xsram[326] sram->in sram[326]->out sram[326]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[326]->out) 0
.nodeset V(sram[326]->outb) vsp
Xsram[327] sram->in sram[327]->out sram[327]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[327]->out) 0
.nodeset V(sram[327]->outb) vsp
Xsram[328] sram->in sram[328]->out sram[328]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[328]->out) 0
.nodeset V(sram[328]->outb) vsp
Xmux_2level_tapbuf_size4[28] chany[1][1]_midout[0] chany[1][1]_midout[1] chany[1][1]_midout[16] chany[1][1]_midout[17] grid[2][1]_pin[0][3][2] sram[329]->outb sram[329]->out sram[330]->out sram[330]->outb sram[331]->outb sram[331]->out sram[332]->out sram[332]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[28], level=2, select_path_id=0. *****
*****1010*****
Xsram[329] sram->in sram[329]->out sram[329]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[329]->out) 0
.nodeset V(sram[329]->outb) vsp
Xsram[330] sram->in sram[330]->out sram[330]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[330]->out) 0
.nodeset V(sram[330]->outb) vsp
Xsram[331] sram->in sram[331]->out sram[331]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[331]->out) 0
.nodeset V(sram[331]->outb) vsp
Xsram[332] sram->in sram[332]->out sram[332]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[332]->out) 0
.nodeset V(sram[332]->outb) vsp
Xmux_2level_tapbuf_size4[29] chany[1][1]_midout[2] chany[1][1]_midout[3] chany[1][1]_midout[20] chany[1][1]_midout[21] grid[2][1]_pin[0][3][4] sram[333]->outb sram[333]->out sram[334]->out sram[334]->outb sram[335]->outb sram[335]->out sram[336]->out sram[336]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[29], level=2, select_path_id=0. *****
*****1010*****
Xsram[333] sram->in sram[333]->out sram[333]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[333]->out) 0
.nodeset V(sram[333]->outb) vsp
Xsram[334] sram->in sram[334]->out sram[334]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[334]->out) 0
.nodeset V(sram[334]->outb) vsp
Xsram[335] sram->in sram[335]->out sram[335]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[335]->out) 0
.nodeset V(sram[335]->outb) vsp
Xsram[336] sram->in sram[336]->out sram[336]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[336]->out) 0
.nodeset V(sram[336]->outb) vsp
Xmux_2level_tapbuf_size4[30] chany[1][1]_midout[4] chany[1][1]_midout[5] chany[1][1]_midout[22] chany[1][1]_midout[23] grid[2][1]_pin[0][3][6] sram[337]->outb sram[337]->out sram[338]->out sram[338]->outb sram[339]->outb sram[339]->out sram[340]->out sram[340]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[30], level=2, select_path_id=0. *****
*****1010*****
Xsram[337] sram->in sram[337]->out sram[337]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[337]->out) 0
.nodeset V(sram[337]->outb) vsp
Xsram[338] sram->in sram[338]->out sram[338]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[338]->out) 0
.nodeset V(sram[338]->outb) vsp
Xsram[339] sram->in sram[339]->out sram[339]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[339]->out) 0
.nodeset V(sram[339]->outb) vsp
Xsram[340] sram->in sram[340]->out sram[340]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[340]->out) 0
.nodeset V(sram[340]->outb) vsp
Xmux_2level_tapbuf_size4[31] chany[1][1]_midout[10] chany[1][1]_midout[11] chany[1][1]_midout[22] chany[1][1]_midout[23] grid[2][1]_pin[0][3][8] sram[341]->outb sram[341]->out sram[342]->out sram[342]->outb sram[343]->outb sram[343]->out sram[344]->out sram[344]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[31], level=2, select_path_id=0. *****
*****1010*****
Xsram[341] sram->in sram[341]->out sram[341]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[341]->out) 0
.nodeset V(sram[341]->outb) vsp
Xsram[342] sram->in sram[342]->out sram[342]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[342]->out) 0
.nodeset V(sram[342]->outb) vsp
Xsram[343] sram->in sram[343]->out sram[343]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[343]->out) 0
.nodeset V(sram[343]->outb) vsp
Xsram[344] sram->in sram[344]->out sram[344]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[344]->out) 0
.nodeset V(sram[344]->outb) vsp
Xmux_2level_tapbuf_size4[32] chany[1][1]_midout[8] chany[1][1]_midout[9] chany[1][1]_midout[24] chany[1][1]_midout[25] grid[2][1]_pin[0][3][10] sram[345]->outb sram[345]->out sram[346]->out sram[346]->outb sram[347]->outb sram[347]->out sram[348]->out sram[348]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[32], level=2, select_path_id=0. *****
*****1010*****
Xsram[345] sram->in sram[345]->out sram[345]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[345]->out) 0
.nodeset V(sram[345]->outb) vsp
Xsram[346] sram->in sram[346]->out sram[346]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[346]->out) 0
.nodeset V(sram[346]->outb) vsp
Xsram[347] sram->in sram[347]->out sram[347]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[347]->out) 0
.nodeset V(sram[347]->outb) vsp
Xsram[348] sram->in sram[348]->out sram[348]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[348]->out) 0
.nodeset V(sram[348]->outb) vsp
Xmux_2level_tapbuf_size4[33] chany[1][1]_midout[14] chany[1][1]_midout[15] chany[1][1]_midout[26] chany[1][1]_midout[27] grid[2][1]_pin[0][3][12] sram[349]->outb sram[349]->out sram[350]->out sram[350]->outb sram[351]->outb sram[351]->out sram[352]->out sram[352]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[33], level=2, select_path_id=0. *****
*****1010*****
Xsram[349] sram->in sram[349]->out sram[349]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[349]->out) 0
.nodeset V(sram[349]->outb) vsp
Xsram[350] sram->in sram[350]->out sram[350]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[350]->out) 0
.nodeset V(sram[350]->outb) vsp
Xsram[351] sram->in sram[351]->out sram[351]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[351]->out) 0
.nodeset V(sram[351]->outb) vsp
Xsram[352] sram->in sram[352]->out sram[352]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[352]->out) 0
.nodeset V(sram[352]->outb) vsp
Xmux_2level_tapbuf_size4[34] chany[1][1]_midout[12] chany[1][1]_midout[13] chany[1][1]_midout[28] chany[1][1]_midout[29] grid[2][1]_pin[0][3][14] sram[353]->outb sram[353]->out sram[354]->out sram[354]->outb sram[355]->outb sram[355]->out sram[356]->out sram[356]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[34], level=2, select_path_id=0. *****
*****1010*****
Xsram[353] sram->in sram[353]->out sram[353]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[353]->out) 0
.nodeset V(sram[353]->outb) vsp
Xsram[354] sram->in sram[354]->out sram[354]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[354]->out) 0
.nodeset V(sram[354]->outb) vsp
Xsram[355] sram->in sram[355]->out sram[355]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[355]->out) 0
.nodeset V(sram[355]->outb) vsp
Xsram[356] sram->in sram[356]->out sram[356]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[356]->out) 0
.nodeset V(sram[356]->outb) vsp
Xmux_2level_tapbuf_size4[35] chany[1][1]_midout[0] chany[1][1]_midout[1] chany[1][1]_midout[16] chany[1][1]_midout[17] grid[1][1]_pin[0][1][1] sram[357]->outb sram[357]->out sram[358]->out sram[358]->outb sram[359]->outb sram[359]->out sram[360]->out sram[360]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[35], level=2, select_path_id=0. *****
*****1010*****
Xsram[357] sram->in sram[357]->out sram[357]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[357]->out) 0
.nodeset V(sram[357]->outb) vsp
Xsram[358] sram->in sram[358]->out sram[358]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[358]->out) 0
.nodeset V(sram[358]->outb) vsp
Xsram[359] sram->in sram[359]->out sram[359]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[359]->out) 0
.nodeset V(sram[359]->outb) vsp
Xsram[360] sram->in sram[360]->out sram[360]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[360]->out) 0
.nodeset V(sram[360]->outb) vsp
.eom
