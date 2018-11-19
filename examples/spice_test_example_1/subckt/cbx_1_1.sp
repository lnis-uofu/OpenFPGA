*****************************
*     FPGA SPICE Netlist    *
* Description: Connection Block X-channel  [1][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
.subckt cbx[1][1] 
+ chanx[1][1]_midout[0] 
+ chanx[1][1]_midout[1] 
+ chanx[1][1]_midout[2] 
+ chanx[1][1]_midout[3] 
+ chanx[1][1]_midout[4] 
+ chanx[1][1]_midout[5] 
+ chanx[1][1]_midout[6] 
+ chanx[1][1]_midout[7] 
+ chanx[1][1]_midout[8] 
+ chanx[1][1]_midout[9] 
+ chanx[1][1]_midout[10] 
+ chanx[1][1]_midout[11] 
+ chanx[1][1]_midout[12] 
+ chanx[1][1]_midout[13] 
+ chanx[1][1]_midout[14] 
+ chanx[1][1]_midout[15] 
+ chanx[1][1]_midout[16] 
+ chanx[1][1]_midout[17] 
+ chanx[1][1]_midout[18] 
+ chanx[1][1]_midout[19] 
+ chanx[1][1]_midout[20] 
+ chanx[1][1]_midout[21] 
+ chanx[1][1]_midout[22] 
+ chanx[1][1]_midout[23] 
+ chanx[1][1]_midout[24] 
+ chanx[1][1]_midout[25] 
+ chanx[1][1]_midout[26] 
+ chanx[1][1]_midout[27] 
+ chanx[1][1]_midout[28] 
+ chanx[1][1]_midout[29] 
+ grid[1][2]_pin[0][2][0] 
+ grid[1][2]_pin[0][2][2] 
+ grid[1][2]_pin[0][2][4] 
+ grid[1][2]_pin[0][2][6] 
+ grid[1][2]_pin[0][2][8] 
+ grid[1][2]_pin[0][2][10] 
+ grid[1][2]_pin[0][2][12] 
+ grid[1][2]_pin[0][2][14] 
+ grid[1][1]_pin[0][0][0] 
+ svdd sgnd
Xmux_2level_tapbuf_size4[9] chanx[1][1]_midout[6] chanx[1][1]_midout[7] chanx[1][1]_midout[12] chanx[1][1]_midout[13] grid[1][2]_pin[0][2][0] sram[253]->outb sram[253]->out sram[254]->out sram[254]->outb sram[255]->outb sram[255]->out sram[256]->out sram[256]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[9], level=2, select_path_id=0. *****
*****1010*****
Xsram[253] sram->in sram[253]->out sram[253]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[253]->out) 0
.nodeset V(sram[253]->outb) vsp
Xsram[254] sram->in sram[254]->out sram[254]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[254]->out) 0
.nodeset V(sram[254]->outb) vsp
Xsram[255] sram->in sram[255]->out sram[255]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[255]->out) 0
.nodeset V(sram[255]->outb) vsp
Xsram[256] sram->in sram[256]->out sram[256]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[256]->out) 0
.nodeset V(sram[256]->outb) vsp
Xmux_2level_tapbuf_size4[10] chanx[1][1]_midout[0] chanx[1][1]_midout[1] chanx[1][1]_midout[18] chanx[1][1]_midout[19] grid[1][2]_pin[0][2][2] sram[257]->outb sram[257]->out sram[258]->out sram[258]->outb sram[259]->outb sram[259]->out sram[260]->out sram[260]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[10], level=2, select_path_id=0. *****
*****1010*****
Xsram[257] sram->in sram[257]->out sram[257]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[257]->out) 0
.nodeset V(sram[257]->outb) vsp
Xsram[258] sram->in sram[258]->out sram[258]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[258]->out) 0
.nodeset V(sram[258]->outb) vsp
Xsram[259] sram->in sram[259]->out sram[259]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[259]->out) 0
.nodeset V(sram[259]->outb) vsp
Xsram[260] sram->in sram[260]->out sram[260]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[260]->out) 0
.nodeset V(sram[260]->outb) vsp
Xmux_2level_tapbuf_size4[11] chanx[1][1]_midout[2] chanx[1][1]_midout[3] chanx[1][1]_midout[16] chanx[1][1]_midout[17] grid[1][2]_pin[0][2][4] sram[261]->outb sram[261]->out sram[262]->out sram[262]->outb sram[263]->outb sram[263]->out sram[264]->out sram[264]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[11], level=2, select_path_id=0. *****
*****1010*****
Xsram[261] sram->in sram[261]->out sram[261]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[261]->out) 0
.nodeset V(sram[261]->outb) vsp
Xsram[262] sram->in sram[262]->out sram[262]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[262]->out) 0
.nodeset V(sram[262]->outb) vsp
Xsram[263] sram->in sram[263]->out sram[263]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[263]->out) 0
.nodeset V(sram[263]->outb) vsp
Xsram[264] sram->in sram[264]->out sram[264]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[264]->out) 0
.nodeset V(sram[264]->outb) vsp
Xmux_2level_tapbuf_size4[12] chanx[1][1]_midout[4] chanx[1][1]_midout[5] chanx[1][1]_midout[20] chanx[1][1]_midout[21] grid[1][2]_pin[0][2][6] sram[265]->outb sram[265]->out sram[266]->out sram[266]->outb sram[267]->outb sram[267]->out sram[268]->out sram[268]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[12], level=2, select_path_id=0. *****
*****1010*****
Xsram[265] sram->in sram[265]->out sram[265]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[265]->out) 0
.nodeset V(sram[265]->outb) vsp
Xsram[266] sram->in sram[266]->out sram[266]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[266]->out) 0
.nodeset V(sram[266]->outb) vsp
Xsram[267] sram->in sram[267]->out sram[267]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[267]->out) 0
.nodeset V(sram[267]->outb) vsp
Xsram[268] sram->in sram[268]->out sram[268]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[268]->out) 0
.nodeset V(sram[268]->outb) vsp
Xmux_2level_tapbuf_size4[13] chanx[1][1]_midout[10] chanx[1][1]_midout[11] chanx[1][1]_midout[22] chanx[1][1]_midout[23] grid[1][2]_pin[0][2][8] sram[269]->outb sram[269]->out sram[270]->out sram[270]->outb sram[271]->outb sram[271]->out sram[272]->out sram[272]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[13], level=2, select_path_id=0. *****
*****1010*****
Xsram[269] sram->in sram[269]->out sram[269]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[269]->out) 0
.nodeset V(sram[269]->outb) vsp
Xsram[270] sram->in sram[270]->out sram[270]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[270]->out) 0
.nodeset V(sram[270]->outb) vsp
Xsram[271] sram->in sram[271]->out sram[271]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[271]->out) 0
.nodeset V(sram[271]->outb) vsp
Xsram[272] sram->in sram[272]->out sram[272]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[272]->out) 0
.nodeset V(sram[272]->outb) vsp
Xmux_2level_tapbuf_size4[14] chanx[1][1]_midout[8] chanx[1][1]_midout[9] chanx[1][1]_midout[24] chanx[1][1]_midout[25] grid[1][2]_pin[0][2][10] sram[273]->outb sram[273]->out sram[274]->out sram[274]->outb sram[275]->outb sram[275]->out sram[276]->out sram[276]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[14], level=2, select_path_id=0. *****
*****1010*****
Xsram[273] sram->in sram[273]->out sram[273]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[273]->out) 0
.nodeset V(sram[273]->outb) vsp
Xsram[274] sram->in sram[274]->out sram[274]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[274]->out) 0
.nodeset V(sram[274]->outb) vsp
Xsram[275] sram->in sram[275]->out sram[275]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[275]->out) 0
.nodeset V(sram[275]->outb) vsp
Xsram[276] sram->in sram[276]->out sram[276]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[276]->out) 0
.nodeset V(sram[276]->outb) vsp
Xmux_2level_tapbuf_size4[15] chanx[1][1]_midout[14] chanx[1][1]_midout[15] chanx[1][1]_midout[26] chanx[1][1]_midout[27] grid[1][2]_pin[0][2][12] sram[277]->outb sram[277]->out sram[278]->out sram[278]->outb sram[279]->outb sram[279]->out sram[280]->out sram[280]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[15], level=2, select_path_id=0. *****
*****1010*****
Xsram[277] sram->in sram[277]->out sram[277]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[277]->out) 0
.nodeset V(sram[277]->outb) vsp
Xsram[278] sram->in sram[278]->out sram[278]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[278]->out) 0
.nodeset V(sram[278]->outb) vsp
Xsram[279] sram->in sram[279]->out sram[279]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[279]->out) 0
.nodeset V(sram[279]->outb) vsp
Xsram[280] sram->in sram[280]->out sram[280]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[280]->out) 0
.nodeset V(sram[280]->outb) vsp
Xmux_2level_tapbuf_size4[16] chanx[1][1]_midout[12] chanx[1][1]_midout[13] chanx[1][1]_midout[28] chanx[1][1]_midout[29] grid[1][2]_pin[0][2][14] sram[281]->outb sram[281]->out sram[282]->out sram[282]->outb sram[283]->outb sram[283]->out sram[284]->out sram[284]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[16], level=2, select_path_id=0. *****
*****1010*****
Xsram[281] sram->in sram[281]->out sram[281]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[281]->out) 0
.nodeset V(sram[281]->outb) vsp
Xsram[282] sram->in sram[282]->out sram[282]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[282]->out) 0
.nodeset V(sram[282]->outb) vsp
Xsram[283] sram->in sram[283]->out sram[283]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[283]->out) 0
.nodeset V(sram[283]->outb) vsp
Xsram[284] sram->in sram[284]->out sram[284]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[284]->out) 0
.nodeset V(sram[284]->outb) vsp
Xmux_2level_tapbuf_size4[17] chanx[1][1]_midout[6] chanx[1][1]_midout[7] chanx[1][1]_midout[12] chanx[1][1]_midout[13] grid[1][1]_pin[0][0][0] sram[285]->outb sram[285]->out sram[286]->out sram[286]->outb sram[287]->outb sram[287]->out sram[288]->out sram[288]->outb svdd sgnd mux_2level_tapbuf_size4
***** SRAM bits for MUX[17], level=2, select_path_id=0. *****
*****1010*****
Xsram[285] sram->in sram[285]->out sram[285]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[285]->out) 0
.nodeset V(sram[285]->outb) vsp
Xsram[286] sram->in sram[286]->out sram[286]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[286]->out) 0
.nodeset V(sram[286]->outb) vsp
Xsram[287] sram->in sram[287]->out sram[287]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[287]->out) 0
.nodeset V(sram[287]->outb) vsp
Xsram[288] sram->in sram[288]->out sram[288]->outb gvdd_sram_cbs sgnd  sram6T
.nodeset V(sram[288]->out) 0
.nodeset V(sram[288]->outb) vsp
.eom
