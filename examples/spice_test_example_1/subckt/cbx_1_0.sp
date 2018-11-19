*****************************
*     FPGA SPICE Netlist    *
* Description: Connection Block X-channel  [1][0] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
.subckt cbx[1][0] 
+ chanx[1][0]_midout[0] 
+ chanx[1][0]_midout[1] 
+ chanx[1][0]_midout[2] 
+ chanx[1][0]_midout[3] 
+ chanx[1][0]_midout[4] 
+ chanx[1][0]_midout[5] 
+ chanx[1][0]_midout[6] 
+ chanx[1][0]_midout[7] 
+ chanx[1][0]_midout[8] 
+ chanx[1][0]_midout[9] 
+ chanx[1][0]_midout[10] 
+ chanx[1][0]_midout[11] 
+ chanx[1][0]_midout[12] 
+ chanx[1][0]_midout[13] 
+ chanx[1][0]_midout[14] 
+ chanx[1][0]_midout[15] 
+ chanx[1][0]_midout[16] 
+ chanx[1][0]_midout[17] 
+ chanx[1][0]_midout[18] 
+ chanx[1][0]_midout[19] 
+ chanx[1][0]_midout[20] 
+ chanx[1][0]_midout[21] 
+ chanx[1][0]_midout[22] 
+ chanx[1][0]_midout[23] 
+ chanx[1][0]_midout[24] 
+ chanx[1][0]_midout[25] 
+ chanx[1][0]_midout[26] 
+ chanx[1][0]_midout[27] 
+ chanx[1][0]_midout[28] 
+ chanx[1][0]_midout[29] 
+ grid[1][1]_pin[0][2][2] 
+ grid[1][0]_pin[0][0][0] 
+ grid[1][0]_pin[0][0][2] 
+ grid[1][0]_pin[0][0][4] 
+ grid[1][0]_pin[0][0][6] 
+ grid[1][0]_pin[0][0][8] 
+ grid[1][0]_pin[0][0][10] 
+ grid[1][0]_pin[0][0][12] 
+ grid[1][0]_pin[0][0][14] 
+ svdd sgnd
Xmux_2level_tapbuf_size4[0] chanx[1][0]_midout[6] chanx[1][0]_midout[7] chanx[1][0]_midout[22] chanx[1][0]_midout[23] grid[1][1]_pin[0][2][2] sram[217]->outb sram[217]->out sram[218]->out sram[218]->outb sram[219]->outb sram[219]->out sram[220]->out sram[220]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[0], level=2, select_path_id=0. *****
*****1010*****
Xsram[217] sram->in sram[217]->out sram[217]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[217]->out) 0
.nodeset V(sram[217]->outb) vsp
Xsram[218] sram->in sram[218]->out sram[218]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[218]->out) 0
.nodeset V(sram[218]->outb) vsp
Xsram[219] sram->in sram[219]->out sram[219]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[219]->out) 0
.nodeset V(sram[219]->outb) vsp
Xsram[220] sram->in sram[220]->out sram[220]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[220]->out) 0
.nodeset V(sram[220]->outb) vsp
Xmux_2level_tapbuf_size4[1] chanx[1][0]_midout[0] chanx[1][0]_midout[1] chanx[1][0]_midout[14] chanx[1][0]_midout[15] grid[1][0]_pin[0][0][0] sram[221]->outb sram[221]->out sram[222]->out sram[222]->outb sram[223]->outb sram[223]->out sram[224]->out sram[224]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[1], level=2, select_path_id=0. *****
*****1010*****
Xsram[221] sram->in sram[221]->out sram[221]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[221]->out) 0
.nodeset V(sram[221]->outb) vsp
Xsram[222] sram->in sram[222]->out sram[222]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[222]->out) 0
.nodeset V(sram[222]->outb) vsp
Xsram[223] sram->in sram[223]->out sram[223]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[223]->out) 0
.nodeset V(sram[223]->outb) vsp
Xsram[224] sram->in sram[224]->out sram[224]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[224]->out) 0
.nodeset V(sram[224]->outb) vsp
Xmux_2level_tapbuf_size4[2] chanx[1][0]_midout[0] chanx[1][0]_midout[1] chanx[1][0]_midout[16] chanx[1][0]_midout[17] grid[1][0]_pin[0][0][2] sram[225]->outb sram[225]->out sram[226]->out sram[226]->outb sram[227]->outb sram[227]->out sram[228]->out sram[228]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[2], level=2, select_path_id=0. *****
*****1010*****
Xsram[225] sram->in sram[225]->out sram[225]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[225]->out) 0
.nodeset V(sram[225]->outb) vsp
Xsram[226] sram->in sram[226]->out sram[226]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[226]->out) 0
.nodeset V(sram[226]->outb) vsp
Xsram[227] sram->in sram[227]->out sram[227]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[227]->out) 0
.nodeset V(sram[227]->outb) vsp
Xsram[228] sram->in sram[228]->out sram[228]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[228]->out) 0
.nodeset V(sram[228]->outb) vsp
Xmux_2level_tapbuf_size4[3] chanx[1][0]_midout[2] chanx[1][0]_midout[3] chanx[1][0]_midout[18] chanx[1][0]_midout[19] grid[1][0]_pin[0][0][4] sram[229]->outb sram[229]->out sram[230]->out sram[230]->outb sram[231]->outb sram[231]->out sram[232]->out sram[232]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[3], level=2, select_path_id=0. *****
*****1010*****
Xsram[229] sram->in sram[229]->out sram[229]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[229]->out) 0
.nodeset V(sram[229]->outb) vsp
Xsram[230] sram->in sram[230]->out sram[230]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[230]->out) 0
.nodeset V(sram[230]->outb) vsp
Xsram[231] sram->in sram[231]->out sram[231]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[231]->out) 0
.nodeset V(sram[231]->outb) vsp
Xsram[232] sram->in sram[232]->out sram[232]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[232]->out) 0
.nodeset V(sram[232]->outb) vsp
Xmux_2level_tapbuf_size4[4] chanx[1][0]_midout[4] chanx[1][0]_midout[5] chanx[1][0]_midout[20] chanx[1][0]_midout[21] grid[1][0]_pin[0][0][6] sram[233]->outb sram[233]->out sram[234]->out sram[234]->outb sram[235]->outb sram[235]->out sram[236]->out sram[236]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[4], level=2, select_path_id=0. *****
*****1010*****
Xsram[233] sram->in sram[233]->out sram[233]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[233]->out) 0
.nodeset V(sram[233]->outb) vsp
Xsram[234] sram->in sram[234]->out sram[234]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[234]->out) 0
.nodeset V(sram[234]->outb) vsp
Xsram[235] sram->in sram[235]->out sram[235]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[235]->out) 0
.nodeset V(sram[235]->outb) vsp
Xsram[236] sram->in sram[236]->out sram[236]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[236]->out) 0
.nodeset V(sram[236]->outb) vsp
Xmux_2level_tapbuf_size4[5] chanx[1][0]_midout[6] chanx[1][0]_midout[7] chanx[1][0]_midout[22] chanx[1][0]_midout[23] grid[1][0]_pin[0][0][8] sram[237]->outb sram[237]->out sram[238]->out sram[238]->outb sram[239]->outb sram[239]->out sram[240]->out sram[240]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[5], level=2, select_path_id=0. *****
*****1010*****
Xsram[237] sram->in sram[237]->out sram[237]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[237]->out) 0
.nodeset V(sram[237]->outb) vsp
Xsram[238] sram->in sram[238]->out sram[238]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[238]->out) 0
.nodeset V(sram[238]->outb) vsp
Xsram[239] sram->in sram[239]->out sram[239]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[239]->out) 0
.nodeset V(sram[239]->outb) vsp
Xsram[240] sram->in sram[240]->out sram[240]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[240]->out) 0
.nodeset V(sram[240]->outb) vsp
Xmux_2level_tapbuf_size4[6] chanx[1][0]_midout[8] chanx[1][0]_midout[9] chanx[1][0]_midout[24] chanx[1][0]_midout[25] grid[1][0]_pin[0][0][10] sram[241]->outb sram[241]->out sram[242]->out sram[242]->outb sram[243]->outb sram[243]->out sram[244]->out sram[244]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[6], level=2, select_path_id=0. *****
*****1010*****
Xsram[241] sram->in sram[241]->out sram[241]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[241]->out) 0
.nodeset V(sram[241]->outb) vsp
Xsram[242] sram->in sram[242]->out sram[242]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[242]->out) 0
.nodeset V(sram[242]->outb) vsp
Xsram[243] sram->in sram[243]->out sram[243]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[243]->out) 0
.nodeset V(sram[243]->outb) vsp
Xsram[244] sram->in sram[244]->out sram[244]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[244]->out) 0
.nodeset V(sram[244]->outb) vsp
Xmux_2level_tapbuf_size4[7] chanx[1][0]_midout[10] chanx[1][0]_midout[11] chanx[1][0]_midout[26] chanx[1][0]_midout[27] grid[1][0]_pin[0][0][12] sram[245]->outb sram[245]->out sram[246]->out sram[246]->outb sram[247]->outb sram[247]->out sram[248]->out sram[248]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[7], level=2, select_path_id=0. *****
*****1010*****
Xsram[245] sram->in sram[245]->out sram[245]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[245]->out) 0
.nodeset V(sram[245]->outb) vsp
Xsram[246] sram->in sram[246]->out sram[246]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[246]->out) 0
.nodeset V(sram[246]->outb) vsp
Xsram[247] sram->in sram[247]->out sram[247]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[247]->out) 0
.nodeset V(sram[247]->outb) vsp
Xsram[248] sram->in sram[248]->out sram[248]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[248]->out) 0
.nodeset V(sram[248]->outb) vsp
Xmux_2level_tapbuf_size4[8] chanx[1][0]_midout[12] chanx[1][0]_midout[13] chanx[1][0]_midout[28] chanx[1][0]_midout[29] grid[1][0]_pin[0][0][14] sram[249]->outb sram[249]->out sram[250]->out sram[250]->outb sram[251]->outb sram[251]->out sram[252]->out sram[252]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[8], level=2, select_path_id=0. *****
*****1010*****
Xsram[249] sram->in sram[249]->out sram[249]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[249]->out) 0
.nodeset V(sram[249]->outb) vsp
Xsram[250] sram->in sram[250]->out sram[250]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[250]->out) 0
.nodeset V(sram[250]->outb) vsp
Xsram[251] sram->in sram[251]->out sram[251]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[251]->out) 0
.nodeset V(sram[251]->outb) vsp
Xsram[252] sram->in sram[252]->out sram[252]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[252]->out) 0
.nodeset V(sram[252]->outb) vsp
.eom
