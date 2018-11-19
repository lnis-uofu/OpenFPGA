*****************************
*     FPGA SPICE Netlist    *
* Description: Connection Block Y-channel  [0][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
.subckt cby[0][1] 
+ chany[0][1]_midout[0] 
+ chany[0][1]_midout[1] 
+ chany[0][1]_midout[2] 
+ chany[0][1]_midout[3] 
+ chany[0][1]_midout[4] 
+ chany[0][1]_midout[5] 
+ chany[0][1]_midout[6] 
+ chany[0][1]_midout[7] 
+ chany[0][1]_midout[8] 
+ chany[0][1]_midout[9] 
+ chany[0][1]_midout[10] 
+ chany[0][1]_midout[11] 
+ chany[0][1]_midout[12] 
+ chany[0][1]_midout[13] 
+ chany[0][1]_midout[14] 
+ chany[0][1]_midout[15] 
+ chany[0][1]_midout[16] 
+ chany[0][1]_midout[17] 
+ chany[0][1]_midout[18] 
+ chany[0][1]_midout[19] 
+ chany[0][1]_midout[20] 
+ chany[0][1]_midout[21] 
+ chany[0][1]_midout[22] 
+ chany[0][1]_midout[23] 
+ chany[0][1]_midout[24] 
+ chany[0][1]_midout[25] 
+ chany[0][1]_midout[26] 
+ chany[0][1]_midout[27] 
+ chany[0][1]_midout[28] 
+ chany[0][1]_midout[29] 
+ grid[1][1]_pin[0][3][3] 
+ grid[0][1]_pin[0][1][0] 
+ grid[0][1]_pin[0][1][2] 
+ grid[0][1]_pin[0][1][4] 
+ grid[0][1]_pin[0][1][6] 
+ grid[0][1]_pin[0][1][8] 
+ grid[0][1]_pin[0][1][10] 
+ grid[0][1]_pin[0][1][12] 
+ grid[0][1]_pin[0][1][14] 
+ svdd sgnd
Xmux_2level_tapbuf_size4[18] chany[0][1]_midout[10] chany[0][1]_midout[11] chany[0][1]_midout[26] chany[0][1]_midout[27] grid[1][1]_pin[0][3][3] sram[289]->out sram[289]->outb sram[290]->outb sram[290]->out sram[291]->out sram[291]->outb sram[292]->outb sram[292]->out svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[18], level=2, select_path_id=3. *****
*****0101*****
Xsram[289] sram->in sram[289]->out sram[289]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[289]->out) 0
.nodeset V(sram[289]->outb) vsp
Xsram[290] sram->in sram[290]->out sram[290]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[290]->out) 0
.nodeset V(sram[290]->outb) vsp
Xsram[291] sram->in sram[291]->out sram[291]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[291]->out) 0
.nodeset V(sram[291]->outb) vsp
Xsram[292] sram->in sram[292]->out sram[292]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[292]->out) 0
.nodeset V(sram[292]->outb) vsp
Xmux_2level_tapbuf_size4[19] chany[0][1]_midout[0] chany[0][1]_midout[1] chany[0][1]_midout[14] chany[0][1]_midout[15] grid[0][1]_pin[0][1][0] sram[293]->outb sram[293]->out sram[294]->out sram[294]->outb sram[295]->outb sram[295]->out sram[296]->out sram[296]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[19], level=2, select_path_id=0. *****
*****1010*****
Xsram[293] sram->in sram[293]->out sram[293]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[293]->out) 0
.nodeset V(sram[293]->outb) vsp
Xsram[294] sram->in sram[294]->out sram[294]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[294]->out) 0
.nodeset V(sram[294]->outb) vsp
Xsram[295] sram->in sram[295]->out sram[295]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[295]->out) 0
.nodeset V(sram[295]->outb) vsp
Xsram[296] sram->in sram[296]->out sram[296]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[296]->out) 0
.nodeset V(sram[296]->outb) vsp
Xmux_2level_tapbuf_size4[20] chany[0][1]_midout[2] chany[0][1]_midout[3] chany[0][1]_midout[16] chany[0][1]_midout[17] grid[0][1]_pin[0][1][2] sram[297]->outb sram[297]->out sram[298]->out sram[298]->outb sram[299]->outb sram[299]->out sram[300]->out sram[300]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[20], level=2, select_path_id=0. *****
*****1010*****
Xsram[297] sram->in sram[297]->out sram[297]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[297]->out) 0
.nodeset V(sram[297]->outb) vsp
Xsram[298] sram->in sram[298]->out sram[298]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[298]->out) 0
.nodeset V(sram[298]->outb) vsp
Xsram[299] sram->in sram[299]->out sram[299]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[299]->out) 0
.nodeset V(sram[299]->outb) vsp
Xsram[300] sram->in sram[300]->out sram[300]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[300]->out) 0
.nodeset V(sram[300]->outb) vsp
Xmux_2level_tapbuf_size4[21] chany[0][1]_midout[4] chany[0][1]_midout[5] chany[0][1]_midout[18] chany[0][1]_midout[19] grid[0][1]_pin[0][1][4] sram[301]->outb sram[301]->out sram[302]->out sram[302]->outb sram[303]->outb sram[303]->out sram[304]->out sram[304]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[21], level=2, select_path_id=0. *****
*****1010*****
Xsram[301] sram->in sram[301]->out sram[301]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[301]->out) 0
.nodeset V(sram[301]->outb) vsp
Xsram[302] sram->in sram[302]->out sram[302]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[302]->out) 0
.nodeset V(sram[302]->outb) vsp
Xsram[303] sram->in sram[303]->out sram[303]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[303]->out) 0
.nodeset V(sram[303]->outb) vsp
Xsram[304] sram->in sram[304]->out sram[304]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[304]->out) 0
.nodeset V(sram[304]->outb) vsp
Xmux_2level_tapbuf_size4[22] chany[0][1]_midout[6] chany[0][1]_midout[7] chany[0][1]_midout[20] chany[0][1]_midout[21] grid[0][1]_pin[0][1][6] sram[305]->outb sram[305]->out sram[306]->out sram[306]->outb sram[307]->outb sram[307]->out sram[308]->out sram[308]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[22], level=2, select_path_id=0. *****
*****1010*****
Xsram[305] sram->in sram[305]->out sram[305]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[305]->out) 0
.nodeset V(sram[305]->outb) vsp
Xsram[306] sram->in sram[306]->out sram[306]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[306]->out) 0
.nodeset V(sram[306]->outb) vsp
Xsram[307] sram->in sram[307]->out sram[307]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[307]->out) 0
.nodeset V(sram[307]->outb) vsp
Xsram[308] sram->in sram[308]->out sram[308]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[308]->out) 0
.nodeset V(sram[308]->outb) vsp
Xmux_2level_tapbuf_size4[23] chany[0][1]_midout[6] chany[0][1]_midout[7] chany[0][1]_midout[22] chany[0][1]_midout[23] grid[0][1]_pin[0][1][8] sram[309]->outb sram[309]->out sram[310]->out sram[310]->outb sram[311]->outb sram[311]->out sram[312]->out sram[312]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[23], level=2, select_path_id=0. *****
*****1010*****
Xsram[309] sram->in sram[309]->out sram[309]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[309]->out) 0
.nodeset V(sram[309]->outb) vsp
Xsram[310] sram->in sram[310]->out sram[310]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[310]->out) 0
.nodeset V(sram[310]->outb) vsp
Xsram[311] sram->in sram[311]->out sram[311]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[311]->out) 0
.nodeset V(sram[311]->outb) vsp
Xsram[312] sram->in sram[312]->out sram[312]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[312]->out) 0
.nodeset V(sram[312]->outb) vsp
Xmux_2level_tapbuf_size4[24] chany[0][1]_midout[8] chany[0][1]_midout[9] chany[0][1]_midout[24] chany[0][1]_midout[25] grid[0][1]_pin[0][1][10] sram[313]->outb sram[313]->out sram[314]->out sram[314]->outb sram[315]->outb sram[315]->out sram[316]->out sram[316]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[24], level=2, select_path_id=0. *****
*****1010*****
Xsram[313] sram->in sram[313]->out sram[313]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[313]->out) 0
.nodeset V(sram[313]->outb) vsp
Xsram[314] sram->in sram[314]->out sram[314]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[314]->out) 0
.nodeset V(sram[314]->outb) vsp
Xsram[315] sram->in sram[315]->out sram[315]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[315]->out) 0
.nodeset V(sram[315]->outb) vsp
Xsram[316] sram->in sram[316]->out sram[316]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[316]->out) 0
.nodeset V(sram[316]->outb) vsp
Xmux_2level_tapbuf_size4[25] chany[0][1]_midout[10] chany[0][1]_midout[11] chany[0][1]_midout[26] chany[0][1]_midout[27] grid[0][1]_pin[0][1][12] sram[317]->outb sram[317]->out sram[318]->out sram[318]->outb sram[319]->outb sram[319]->out sram[320]->out sram[320]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[25], level=2, select_path_id=0. *****
*****1010*****
Xsram[317] sram->in sram[317]->out sram[317]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[317]->out) 0
.nodeset V(sram[317]->outb) vsp
Xsram[318] sram->in sram[318]->out sram[318]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[318]->out) 0
.nodeset V(sram[318]->outb) vsp
Xsram[319] sram->in sram[319]->out sram[319]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[319]->out) 0
.nodeset V(sram[319]->outb) vsp
Xsram[320] sram->in sram[320]->out sram[320]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[320]->out) 0
.nodeset V(sram[320]->outb) vsp
Xmux_2level_tapbuf_size4[26] chany[0][1]_midout[12] chany[0][1]_midout[13] chany[0][1]_midout[28] chany[0][1]_midout[29] grid[0][1]_pin[0][1][14] sram[321]->outb sram[321]->out sram[322]->out sram[322]->outb sram[323]->outb sram[323]->out sram[324]->out sram[324]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[26], level=2, select_path_id=0. *****
*****1010*****
Xsram[321] sram->in sram[321]->out sram[321]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[321]->out) 0
.nodeset V(sram[321]->outb) vsp
Xsram[322] sram->in sram[322]->out sram[322]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[322]->out) 0
.nodeset V(sram[322]->outb) vsp
Xsram[323] sram->in sram[323]->out sram[323]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[323]->out) 0
.nodeset V(sram[323]->outb) vsp
Xsram[324] sram->in sram[324]->out sram[324]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[324]->out) 0
.nodeset V(sram[324]->outb) vsp
.eom
