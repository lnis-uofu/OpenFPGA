*****************************
*     FPGA SPICE Netlist    *
* Description: Logic Block [1][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Grid[1][1] type_descriptor: clb[0] *****
***** Logical block mapped to this LUT: n7 *****
.subckt grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_lut4[0] lut4[0]->in[0] lut4[0]->in[1] lut4[0]->in[2] lut4[0]->in[3] lut4[0]->out[0] svdd sgnd
***** Truth Table for LUT[0], size=4. *****
* 0--- 1 *
***** SRAM bits for LUT[0], size=4, num_sram=16. *****
*****0101010101010101*****
Xsram[0] sram->in sram[0]->out sram[0]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[0]->out) 0
.nodeset V(sram[0]->outb) vsp
Xsram[1] sram->in sram[1]->out sram[1]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[1]->out) 0
.nodeset V(sram[1]->outb) vsp
Xsram[2] sram->in sram[2]->out sram[2]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[2]->out) 0
.nodeset V(sram[2]->outb) vsp
Xsram[3] sram->in sram[3]->out sram[3]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[3]->out) 0
.nodeset V(sram[3]->outb) vsp
Xsram[4] sram->in sram[4]->out sram[4]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[4]->out) 0
.nodeset V(sram[4]->outb) vsp
Xsram[5] sram->in sram[5]->out sram[5]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[5]->out) 0
.nodeset V(sram[5]->outb) vsp
Xsram[6] sram->in sram[6]->out sram[6]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[6]->out) 0
.nodeset V(sram[6]->outb) vsp
Xsram[7] sram->in sram[7]->out sram[7]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[7]->out) 0
.nodeset V(sram[7]->outb) vsp
Xsram[8] sram->in sram[8]->out sram[8]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[8]->out) 0
.nodeset V(sram[8]->outb) vsp
Xsram[9] sram->in sram[9]->out sram[9]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[9]->out) 0
.nodeset V(sram[9]->outb) vsp
Xsram[10] sram->in sram[10]->out sram[10]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[10]->out) 0
.nodeset V(sram[10]->outb) vsp
Xsram[11] sram->in sram[11]->out sram[11]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[11]->out) 0
.nodeset V(sram[11]->outb) vsp
Xsram[12] sram->in sram[12]->out sram[12]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[12]->out) 0
.nodeset V(sram[12]->outb) vsp
Xsram[13] sram->in sram[13]->out sram[13]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[13]->out) 0
.nodeset V(sram[13]->outb) vsp
Xsram[14] sram->in sram[14]->out sram[14]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[14]->out) 0
.nodeset V(sram[14]->outb) vsp
Xsram[15] sram->in sram[15]->out sram[15]->outb gvdd_sram_luts sgnd  sram6T
.nodeset V(sram[15]->out) 0
.nodeset V(sram[15]->outb) vsp
Xlut4[0] lut4[0]->in[0] lut4[0]->in[1] lut4[0]->in[2] lut4[0]->in[3] lut4[0]->out[0] sram[0]->out sram[1]->outb sram[2]->out sram[3]->outb sram[4]->out sram[5]->outb sram[6]->out sram[7]->outb sram[8]->out sram[9]->outb sram[10]->out sram[11]->outb sram[12]->out sram[13]->outb sram[14]->out sram[15]->outb gvdd_lut4[0] sgnd lut4
.eom
***** Logical block mapped to this FF: Q0 *****
.subckt grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_ff[0] ff[0]->D[0] ff[0]->Q[0] svdd sgnd
Xdff[0] 
***** BEGIN Global ports of SPICE_MODEL(static_dff) *****
+  Set[0]  Reset[0]  clk[0] 
***** END Global ports of SPICE_MODEL(static_dff) *****
+ ff[0]->D[0] ff[0]->Q[0] gvdd_dff[0] sgnd static_dff
.nodeset V(ff[0]->Q[0]) 0
.eom
.subckt grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4] mode[ble4]->in[0] mode[ble4]->in[1] mode[ble4]->in[2] mode[ble4]->in[3] mode[ble4]->out[0] mode[ble4]->clk[0] svdd sgnd
Xlut4[0] lut4[0]->in[0] lut4[0]->in[1] lut4[0]->in[2] lut4[0]->in[3] lut4[0]->out[0] svdd sgnd grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_lut4[0]
Xff[0] ff[0]->D[0] ff[0]->Q[0] svdd sgnd grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]_ff[0]
Xmux_1level_tapbuf_size2[0] ff[0]->Q[0] lut4[0]->out[0] mode[ble4]->out[0] sram[16]->outb sram[16]->out gvdd_local_interc sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[0], level=1, select_path_id=0. *****
*****1*****
Xsram[16] sram->in sram[16]->out sram[16]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[16]->out) 0
.nodeset V(sram[16]->outb) vsp
Xdirect_interc[0] mode[ble4]->in[0] lut4[0]->in[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[1] mode[ble4]->in[1] lut4[0]->in[1] gvdd_local_interc sgnd direct_interc
Xdirect_interc[2] mode[ble4]->in[2] lut4[0]->in[2] gvdd_local_interc sgnd direct_interc
Xdirect_interc[3] mode[ble4]->in[3] lut4[0]->in[3] gvdd_local_interc sgnd direct_interc
Xdirect_interc[4] lut4[0]->out[0] ff[0]->D[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[5] mode[ble4]->clk[0] ff[0]->clk[0] gvdd_local_interc sgnd direct_interc
.eom
.subckt grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4] mode[n1_lut4]->in[0] mode[n1_lut4]->in[1] mode[n1_lut4]->in[2] mode[n1_lut4]->in[3] mode[n1_lut4]->out[0] mode[n1_lut4]->clk[0] svdd sgnd
Xble4[0] ble4[0]->in[0] ble4[0]->in[1] ble4[0]->in[2] ble4[0]->in[3] ble4[0]->out[0] ble4[0]->clk[0] svdd sgnd grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]_ble4[0]_mode[ble4]
Xdirect_interc[6] ble4[0]->out[0] mode[n1_lut4]->out[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[7] mode[n1_lut4]->in[0] ble4[0]->in[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[8] mode[n1_lut4]->in[1] ble4[0]->in[1] gvdd_local_interc sgnd direct_interc
Xdirect_interc[9] mode[n1_lut4]->in[2] ble4[0]->in[2] gvdd_local_interc sgnd direct_interc
Xdirect_interc[10] mode[n1_lut4]->in[3] ble4[0]->in[3] gvdd_local_interc sgnd direct_interc
Xdirect_interc[11] mode[n1_lut4]->clk[0] ble4[0]->clk[0] gvdd_local_interc sgnd direct_interc
.eom
.subckt grid[1][1]_clb[0]_mode[clb] mode[clb]->I[0] mode[clb]->I[1] mode[clb]->I[2] mode[clb]->I[3] mode[clb]->O[0] mode[clb]->clk[0] svdd sgnd
Xfle[0] fle[0]->in[0] fle[0]->in[1] fle[0]->in[2] fle[0]->in[3] fle[0]->out[0] fle[0]->clk[0] svdd sgnd grid[1][1]_clb[0]_mode[clb]_fle[0]_mode[n1_lut4]
Xdirect_interc[12] fle[0]->out[0] mode[clb]->O[0] gvdd_local_interc sgnd direct_interc
Xmux_2level_size5[0] mode[clb]->I[0] mode[clb]->I[1] mode[clb]->I[2] mode[clb]->I[3] fle[0]->out[0] fle[0]->in[0] sram[17]->out sram[17]->outb sram[18]->outb sram[18]->out sram[19]->out sram[19]->outb sram[20]->outb sram[20]->out sram[21]->out sram[21]->outb sram[22]->out sram[22]->outb gvdd_local_interc sgnd mux_2level_size5
***** SRAM bits for MUX[0], level=2, select_path_id=3. *****
*****010100*****
Xsram[17] sram->in sram[17]->out sram[17]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[17]->out) 0
.nodeset V(sram[17]->outb) vsp
Xsram[18] sram->in sram[18]->out sram[18]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[18]->out) 0
.nodeset V(sram[18]->outb) vsp
Xsram[19] sram->in sram[19]->out sram[19]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[19]->out) 0
.nodeset V(sram[19]->outb) vsp
Xsram[20] sram->in sram[20]->out sram[20]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[20]->out) 0
.nodeset V(sram[20]->outb) vsp
Xsram[21] sram->in sram[21]->out sram[21]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[21]->out) 0
.nodeset V(sram[21]->outb) vsp
Xsram[22] sram->in sram[22]->out sram[22]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[22]->out) 0
.nodeset V(sram[22]->outb) vsp
Xmux_2level_size5[1] mode[clb]->I[0] mode[clb]->I[1] mode[clb]->I[2] mode[clb]->I[3] fle[0]->out[0] fle[0]->in[1] sram[23]->outb sram[23]->out sram[24]->out sram[24]->outb sram[25]->out sram[25]->outb sram[26]->outb sram[26]->out sram[27]->out sram[27]->outb sram[28]->out sram[28]->outb gvdd_local_interc sgnd mux_2level_size5
***** SRAM bits for MUX[1], level=2, select_path_id=0. *****
*****100100*****
Xsram[23] sram->in sram[23]->out sram[23]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[23]->out) 0
.nodeset V(sram[23]->outb) vsp
Xsram[24] sram->in sram[24]->out sram[24]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[24]->out) 0
.nodeset V(sram[24]->outb) vsp
Xsram[25] sram->in sram[25]->out sram[25]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[25]->out) 0
.nodeset V(sram[25]->outb) vsp
Xsram[26] sram->in sram[26]->out sram[26]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[26]->out) 0
.nodeset V(sram[26]->outb) vsp
Xsram[27] sram->in sram[27]->out sram[27]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[27]->out) 0
.nodeset V(sram[27]->outb) vsp
Xsram[28] sram->in sram[28]->out sram[28]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[28]->out) 0
.nodeset V(sram[28]->outb) vsp
Xmux_2level_size5[2] mode[clb]->I[0] mode[clb]->I[1] mode[clb]->I[2] mode[clb]->I[3] fle[0]->out[0] fle[0]->in[2] sram[29]->outb sram[29]->out sram[30]->out sram[30]->outb sram[31]->out sram[31]->outb sram[32]->outb sram[32]->out sram[33]->out sram[33]->outb sram[34]->out sram[34]->outb gvdd_local_interc sgnd mux_2level_size5
***** SRAM bits for MUX[2], level=2, select_path_id=0. *****
*****100100*****
Xsram[29] sram->in sram[29]->out sram[29]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[29]->out) 0
.nodeset V(sram[29]->outb) vsp
Xsram[30] sram->in sram[30]->out sram[30]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[30]->out) 0
.nodeset V(sram[30]->outb) vsp
Xsram[31] sram->in sram[31]->out sram[31]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[31]->out) 0
.nodeset V(sram[31]->outb) vsp
Xsram[32] sram->in sram[32]->out sram[32]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[32]->out) 0
.nodeset V(sram[32]->outb) vsp
Xsram[33] sram->in sram[33]->out sram[33]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[33]->out) 0
.nodeset V(sram[33]->outb) vsp
Xsram[34] sram->in sram[34]->out sram[34]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[34]->out) 0
.nodeset V(sram[34]->outb) vsp
Xmux_2level_size5[3] mode[clb]->I[0] mode[clb]->I[1] mode[clb]->I[2] mode[clb]->I[3] fle[0]->out[0] fle[0]->in[3] sram[35]->outb sram[35]->out sram[36]->out sram[36]->outb sram[37]->out sram[37]->outb sram[38]->outb sram[38]->out sram[39]->out sram[39]->outb sram[40]->out sram[40]->outb gvdd_local_interc sgnd mux_2level_size5
***** SRAM bits for MUX[3], level=2, select_path_id=0. *****
*****100100*****
Xsram[35] sram->in sram[35]->out sram[35]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[35]->out) 0
.nodeset V(sram[35]->outb) vsp
Xsram[36] sram->in sram[36]->out sram[36]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[36]->out) 0
.nodeset V(sram[36]->outb) vsp
Xsram[37] sram->in sram[37]->out sram[37]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[37]->out) 0
.nodeset V(sram[37]->outb) vsp
Xsram[38] sram->in sram[38]->out sram[38]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[38]->out) 0
.nodeset V(sram[38]->outb) vsp
Xsram[39] sram->in sram[39]->out sram[39]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[39]->out) 0
.nodeset V(sram[39]->outb) vsp
Xsram[40] sram->in sram[40]->out sram[40]->outb gvdd_sram_local_routing sgnd  sram6T
.nodeset V(sram[40]->out) 0
.nodeset V(sram[40]->outb) vsp
Xdirect_interc[13] mode[clb]->clk[0] fle[0]->clk[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][1], Capactity: 1 *****
***** Top Protocol *****
.subckt grid[1][1] 
+ top_height[0]_pin[0] 
+ top_height[0]_pin[4] 
+ right_height[0]_pin[1] 
+ right_height[0]_pin[5] 
+ bottom_height[0]_pin[2] 
+ left_height[0]_pin[3] 
+ svdd sgnd
Xgrid[1][1][0] 
+ top_height[0]_pin[0] 
+ right_height[0]_pin[1] 
+ bottom_height[0]_pin[2] 
+ left_height[0]_pin[3] 
+ top_height[0]_pin[4] 
+ right_height[0]_pin[5] 
+ svdd sgnd grid[1][1]_clb[0]_mode[clb]
.eom
