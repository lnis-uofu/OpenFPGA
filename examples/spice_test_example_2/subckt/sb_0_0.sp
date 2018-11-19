*****************************
*     FPGA SPICE Netlist    *
* Description: Switch Block  [0][0] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
***** Switch Box[0][0] Sub-Circuit *****
.subckt sb[0][0] 
***** Inputs/outputs of top side *****
+ chany[0][1]_out[0] chany[0][1]_in[1] chany[0][1]_out[2] chany[0][1]_in[3] chany[0][1]_out[4] chany[0][1]_in[5] chany[0][1]_out[6] chany[0][1]_in[7] chany[0][1]_out[8] chany[0][1]_in[9] chany[0][1]_out[10] chany[0][1]_in[11] chany[0][1]_out[12] chany[0][1]_in[13] chany[0][1]_out[14] chany[0][1]_in[15] chany[0][1]_out[16] chany[0][1]_in[17] chany[0][1]_out[18] chany[0][1]_in[19] chany[0][1]_out[20] chany[0][1]_in[21] chany[0][1]_out[22] chany[0][1]_in[23] chany[0][1]_out[24] chany[0][1]_in[25] chany[0][1]_out[26] chany[0][1]_in[27] chany[0][1]_out[28] chany[0][1]_in[29] chany[0][1]_out[30] chany[0][1]_in[31] chany[0][1]_out[32] chany[0][1]_in[33] chany[0][1]_out[34] chany[0][1]_in[35] chany[0][1]_out[36] chany[0][1]_in[37] chany[0][1]_out[38] chany[0][1]_in[39] chany[0][1]_out[40] chany[0][1]_in[41] chany[0][1]_out[42] chany[0][1]_in[43] chany[0][1]_out[44] chany[0][1]_in[45] chany[0][1]_out[46] chany[0][1]_in[47] chany[0][1]_out[48] chany[0][1]_in[49] chany[0][1]_out[50] chany[0][1]_in[51] chany[0][1]_out[52] chany[0][1]_in[53] chany[0][1]_out[54] chany[0][1]_in[55] chany[0][1]_out[56] chany[0][1]_in[57] chany[0][1]_out[58] chany[0][1]_in[59] chany[0][1]_out[60] chany[0][1]_in[61] chany[0][1]_out[62] chany[0][1]_in[63] chany[0][1]_out[64] chany[0][1]_in[65] chany[0][1]_out[66] chany[0][1]_in[67] chany[0][1]_out[68] chany[0][1]_in[69] chany[0][1]_out[70] chany[0][1]_in[71] chany[0][1]_out[72] chany[0][1]_in[73] chany[0][1]_out[74] chany[0][1]_in[75] chany[0][1]_out[76] chany[0][1]_in[77] chany[0][1]_out[78] chany[0][1]_in[79] chany[0][1]_out[80] chany[0][1]_in[81] chany[0][1]_out[82] chany[0][1]_in[83] chany[0][1]_out[84] chany[0][1]_in[85] chany[0][1]_out[86] chany[0][1]_in[87] chany[0][1]_out[88] chany[0][1]_in[89] chany[0][1]_out[90] chany[0][1]_in[91] chany[0][1]_out[92] chany[0][1]_in[93] chany[0][1]_out[94] chany[0][1]_in[95] chany[0][1]_out[96] chany[0][1]_in[97] chany[0][1]_out[98] chany[0][1]_in[99] 
+ grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][3] grid[0][1]_pin[0][1][5] grid[0][1]_pin[0][1][7] grid[0][1]_pin[0][1][9] grid[0][1]_pin[0][1][11] grid[0][1]_pin[0][1][13] grid[0][1]_pin[0][1][15] grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][47] 
+ ***** Inputs/outputs of right side *****
+ chanx[1][0]_out[0] chanx[1][0]_in[1] chanx[1][0]_out[2] chanx[1][0]_in[3] chanx[1][0]_out[4] chanx[1][0]_in[5] chanx[1][0]_out[6] chanx[1][0]_in[7] chanx[1][0]_out[8] chanx[1][0]_in[9] chanx[1][0]_out[10] chanx[1][0]_in[11] chanx[1][0]_out[12] chanx[1][0]_in[13] chanx[1][0]_out[14] chanx[1][0]_in[15] chanx[1][0]_out[16] chanx[1][0]_in[17] chanx[1][0]_out[18] chanx[1][0]_in[19] chanx[1][0]_out[20] chanx[1][0]_in[21] chanx[1][0]_out[22] chanx[1][0]_in[23] chanx[1][0]_out[24] chanx[1][0]_in[25] chanx[1][0]_out[26] chanx[1][0]_in[27] chanx[1][0]_out[28] chanx[1][0]_in[29] chanx[1][0]_out[30] chanx[1][0]_in[31] chanx[1][0]_out[32] chanx[1][0]_in[33] chanx[1][0]_out[34] chanx[1][0]_in[35] chanx[1][0]_out[36] chanx[1][0]_in[37] chanx[1][0]_out[38] chanx[1][0]_in[39] chanx[1][0]_out[40] chanx[1][0]_in[41] chanx[1][0]_out[42] chanx[1][0]_in[43] chanx[1][0]_out[44] chanx[1][0]_in[45] chanx[1][0]_out[46] chanx[1][0]_in[47] chanx[1][0]_out[48] chanx[1][0]_in[49] chanx[1][0]_out[50] chanx[1][0]_in[51] chanx[1][0]_out[52] chanx[1][0]_in[53] chanx[1][0]_out[54] chanx[1][0]_in[55] chanx[1][0]_out[56] chanx[1][0]_in[57] chanx[1][0]_out[58] chanx[1][0]_in[59] chanx[1][0]_out[60] chanx[1][0]_in[61] chanx[1][0]_out[62] chanx[1][0]_in[63] chanx[1][0]_out[64] chanx[1][0]_in[65] chanx[1][0]_out[66] chanx[1][0]_in[67] chanx[1][0]_out[68] chanx[1][0]_in[69] chanx[1][0]_out[70] chanx[1][0]_in[71] chanx[1][0]_out[72] chanx[1][0]_in[73] chanx[1][0]_out[74] chanx[1][0]_in[75] chanx[1][0]_out[76] chanx[1][0]_in[77] chanx[1][0]_out[78] chanx[1][0]_in[79] chanx[1][0]_out[80] chanx[1][0]_in[81] chanx[1][0]_out[82] chanx[1][0]_in[83] chanx[1][0]_out[84] chanx[1][0]_in[85] chanx[1][0]_out[86] chanx[1][0]_in[87] chanx[1][0]_out[88] chanx[1][0]_in[89] chanx[1][0]_out[90] chanx[1][0]_in[91] chanx[1][0]_out[92] chanx[1][0]_in[93] chanx[1][0]_out[94] chanx[1][0]_in[95] chanx[1][0]_out[96] chanx[1][0]_in[97] chanx[1][0]_out[98] chanx[1][0]_in[99] 
+ grid[1][1]_pin[0][2][42] grid[1][1]_pin[0][2][46] grid[1][0]_pin[0][0][1] grid[1][0]_pin[0][0][3] grid[1][0]_pin[0][0][5] grid[1][0]_pin[0][0][7] grid[1][0]_pin[0][0][9] grid[1][0]_pin[0][0][11] grid[1][0]_pin[0][0][13] grid[1][0]_pin[0][0][15] 
+ ***** Inputs/outputs of bottom side *****
+ 
+ 
+ ***** Inputs/outputs of left side *****
+ 
+ 
+ svdd sgnd
***** top side Multiplexers *****
Xmux_1level_tapbuf_size2[10] grid[0][1]_pin[0][1][1] chanx[1][0]_in[3] chany[0][1]_out[0] sram[1642]->outb sram[1642]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[10], level=1, select_path_id=0. *****
*****1*****
Xsram[1642] sram->in sram[1642]->out sram[1642]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1642]->out) 0
.nodeset V(sram[1642]->outb) vsp
Xmux_1level_tapbuf_size2[11] grid[0][1]_pin[0][1][1] chanx[1][0]_in[5] chany[0][1]_out[2] sram[1643]->outb sram[1643]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[11], level=1, select_path_id=0. *****
*****1*****
Xsram[1643] sram->in sram[1643]->out sram[1643]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1643]->out) 0
.nodeset V(sram[1643]->outb) vsp
Xmux_1level_tapbuf_size2[12] grid[0][1]_pin[0][1][1] chanx[1][0]_in[7] chany[0][1]_out[4] sram[1644]->outb sram[1644]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[12], level=1, select_path_id=0. *****
*****1*****
Xsram[1644] sram->in sram[1644]->out sram[1644]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1644]->out) 0
.nodeset V(sram[1644]->outb) vsp
Xmux_1level_tapbuf_size2[13] grid[0][1]_pin[0][1][1] chanx[1][0]_in[9] chany[0][1]_out[6] sram[1645]->outb sram[1645]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[13], level=1, select_path_id=0. *****
*****1*****
Xsram[1645] sram->in sram[1645]->out sram[1645]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1645]->out) 0
.nodeset V(sram[1645]->outb) vsp
Xmux_1level_tapbuf_size2[14] grid[0][1]_pin[0][1][1] chanx[1][0]_in[11] chany[0][1]_out[8] sram[1646]->outb sram[1646]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[14], level=1, select_path_id=0. *****
*****1*****
Xsram[1646] sram->in sram[1646]->out sram[1646]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1646]->out) 0
.nodeset V(sram[1646]->outb) vsp
Xmux_1level_tapbuf_size2[15] grid[0][1]_pin[0][1][3] chanx[1][0]_in[13] chany[0][1]_out[10] sram[1647]->outb sram[1647]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[15], level=1, select_path_id=0. *****
*****1*****
Xsram[1647] sram->in sram[1647]->out sram[1647]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1647]->out) 0
.nodeset V(sram[1647]->outb) vsp
Xmux_1level_tapbuf_size2[16] grid[0][1]_pin[0][1][3] chanx[1][0]_in[15] chany[0][1]_out[12] sram[1648]->outb sram[1648]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[16], level=1, select_path_id=0. *****
*****1*****
Xsram[1648] sram->in sram[1648]->out sram[1648]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1648]->out) 0
.nodeset V(sram[1648]->outb) vsp
Xmux_1level_tapbuf_size2[17] grid[0][1]_pin[0][1][3] chanx[1][0]_in[17] chany[0][1]_out[14] sram[1649]->outb sram[1649]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[17], level=1, select_path_id=0. *****
*****1*****
Xsram[1649] sram->in sram[1649]->out sram[1649]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1649]->out) 0
.nodeset V(sram[1649]->outb) vsp
Xmux_1level_tapbuf_size2[18] grid[0][1]_pin[0][1][3] chanx[1][0]_in[19] chany[0][1]_out[16] sram[1650]->outb sram[1650]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[18], level=1, select_path_id=0. *****
*****1*****
Xsram[1650] sram->in sram[1650]->out sram[1650]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1650]->out) 0
.nodeset V(sram[1650]->outb) vsp
Xmux_1level_tapbuf_size2[19] grid[0][1]_pin[0][1][3] chanx[1][0]_in[21] chany[0][1]_out[18] sram[1651]->outb sram[1651]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[19], level=1, select_path_id=0. *****
*****1*****
Xsram[1651] sram->in sram[1651]->out sram[1651]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1651]->out) 0
.nodeset V(sram[1651]->outb) vsp
Xmux_1level_tapbuf_size2[20] grid[0][1]_pin[0][1][5] chanx[1][0]_in[23] chany[0][1]_out[20] sram[1652]->outb sram[1652]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[20], level=1, select_path_id=0. *****
*****1*****
Xsram[1652] sram->in sram[1652]->out sram[1652]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1652]->out) 0
.nodeset V(sram[1652]->outb) vsp
Xmux_1level_tapbuf_size2[21] grid[0][1]_pin[0][1][5] chanx[1][0]_in[25] chany[0][1]_out[22] sram[1653]->outb sram[1653]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[21], level=1, select_path_id=0. *****
*****1*****
Xsram[1653] sram->in sram[1653]->out sram[1653]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1653]->out) 0
.nodeset V(sram[1653]->outb) vsp
Xmux_1level_tapbuf_size2[22] grid[0][1]_pin[0][1][5] chanx[1][0]_in[27] chany[0][1]_out[24] sram[1654]->outb sram[1654]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[22], level=1, select_path_id=0. *****
*****1*****
Xsram[1654] sram->in sram[1654]->out sram[1654]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1654]->out) 0
.nodeset V(sram[1654]->outb) vsp
Xmux_1level_tapbuf_size2[23] grid[0][1]_pin[0][1][5] chanx[1][0]_in[29] chany[0][1]_out[26] sram[1655]->outb sram[1655]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[23], level=1, select_path_id=0. *****
*****1*****
Xsram[1655] sram->in sram[1655]->out sram[1655]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1655]->out) 0
.nodeset V(sram[1655]->outb) vsp
Xmux_1level_tapbuf_size2[24] grid[0][1]_pin[0][1][5] chanx[1][0]_in[31] chany[0][1]_out[28] sram[1656]->outb sram[1656]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[24], level=1, select_path_id=0. *****
*****1*****
Xsram[1656] sram->in sram[1656]->out sram[1656]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1656]->out) 0
.nodeset V(sram[1656]->outb) vsp
Xmux_1level_tapbuf_size2[25] grid[0][1]_pin[0][1][7] chanx[1][0]_in[33] chany[0][1]_out[30] sram[1657]->outb sram[1657]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[25], level=1, select_path_id=0. *****
*****1*****
Xsram[1657] sram->in sram[1657]->out sram[1657]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1657]->out) 0
.nodeset V(sram[1657]->outb) vsp
Xmux_1level_tapbuf_size2[26] grid[0][1]_pin[0][1][7] chanx[1][0]_in[35] chany[0][1]_out[32] sram[1658]->outb sram[1658]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[26], level=1, select_path_id=0. *****
*****1*****
Xsram[1658] sram->in sram[1658]->out sram[1658]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1658]->out) 0
.nodeset V(sram[1658]->outb) vsp
Xmux_1level_tapbuf_size2[27] grid[0][1]_pin[0][1][7] chanx[1][0]_in[37] chany[0][1]_out[34] sram[1659]->outb sram[1659]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[27], level=1, select_path_id=0. *****
*****1*****
Xsram[1659] sram->in sram[1659]->out sram[1659]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1659]->out) 0
.nodeset V(sram[1659]->outb) vsp
Xmux_1level_tapbuf_size2[28] grid[0][1]_pin[0][1][7] chanx[1][0]_in[39] chany[0][1]_out[36] sram[1660]->outb sram[1660]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[28], level=1, select_path_id=0. *****
*****1*****
Xsram[1660] sram->in sram[1660]->out sram[1660]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1660]->out) 0
.nodeset V(sram[1660]->outb) vsp
Xmux_1level_tapbuf_size2[29] grid[0][1]_pin[0][1][7] chanx[1][0]_in[41] chany[0][1]_out[38] sram[1661]->outb sram[1661]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[29], level=1, select_path_id=0. *****
*****1*****
Xsram[1661] sram->in sram[1661]->out sram[1661]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1661]->out) 0
.nodeset V(sram[1661]->outb) vsp
Xmux_1level_tapbuf_size2[30] grid[0][1]_pin[0][1][9] chanx[1][0]_in[43] chany[0][1]_out[40] sram[1662]->outb sram[1662]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[30], level=1, select_path_id=0. *****
*****1*****
Xsram[1662] sram->in sram[1662]->out sram[1662]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1662]->out) 0
.nodeset V(sram[1662]->outb) vsp
Xmux_1level_tapbuf_size2[31] grid[0][1]_pin[0][1][9] chanx[1][0]_in[45] chany[0][1]_out[42] sram[1663]->outb sram[1663]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[31], level=1, select_path_id=0. *****
*****1*****
Xsram[1663] sram->in sram[1663]->out sram[1663]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1663]->out) 0
.nodeset V(sram[1663]->outb) vsp
Xmux_1level_tapbuf_size2[32] grid[0][1]_pin[0][1][9] chanx[1][0]_in[47] chany[0][1]_out[44] sram[1664]->outb sram[1664]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[32], level=1, select_path_id=0. *****
*****1*****
Xsram[1664] sram->in sram[1664]->out sram[1664]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1664]->out) 0
.nodeset V(sram[1664]->outb) vsp
Xmux_1level_tapbuf_size2[33] grid[0][1]_pin[0][1][9] chanx[1][0]_in[49] chany[0][1]_out[46] sram[1665]->outb sram[1665]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[33], level=1, select_path_id=0. *****
*****1*****
Xsram[1665] sram->in sram[1665]->out sram[1665]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1665]->out) 0
.nodeset V(sram[1665]->outb) vsp
Xmux_1level_tapbuf_size2[34] grid[0][1]_pin[0][1][9] chanx[1][0]_in[51] chany[0][1]_out[48] sram[1666]->outb sram[1666]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[34], level=1, select_path_id=0. *****
*****1*****
Xsram[1666] sram->in sram[1666]->out sram[1666]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1666]->out) 0
.nodeset V(sram[1666]->outb) vsp
Xmux_1level_tapbuf_size2[35] grid[0][1]_pin[0][1][11] chanx[1][0]_in[53] chany[0][1]_out[50] sram[1667]->outb sram[1667]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[35], level=1, select_path_id=0. *****
*****1*****
Xsram[1667] sram->in sram[1667]->out sram[1667]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1667]->out) 0
.nodeset V(sram[1667]->outb) vsp
Xmux_1level_tapbuf_size2[36] grid[0][1]_pin[0][1][11] chanx[1][0]_in[55] chany[0][1]_out[52] sram[1668]->outb sram[1668]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[36], level=1, select_path_id=0. *****
*****1*****
Xsram[1668] sram->in sram[1668]->out sram[1668]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1668]->out) 0
.nodeset V(sram[1668]->outb) vsp
Xmux_1level_tapbuf_size2[37] grid[0][1]_pin[0][1][11] chanx[1][0]_in[57] chany[0][1]_out[54] sram[1669]->outb sram[1669]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[37], level=1, select_path_id=0. *****
*****1*****
Xsram[1669] sram->in sram[1669]->out sram[1669]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1669]->out) 0
.nodeset V(sram[1669]->outb) vsp
Xmux_1level_tapbuf_size2[38] grid[0][1]_pin[0][1][11] chanx[1][0]_in[59] chany[0][1]_out[56] sram[1670]->outb sram[1670]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[38], level=1, select_path_id=0. *****
*****1*****
Xsram[1670] sram->in sram[1670]->out sram[1670]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1670]->out) 0
.nodeset V(sram[1670]->outb) vsp
Xmux_1level_tapbuf_size2[39] grid[0][1]_pin[0][1][11] chanx[1][0]_in[61] chany[0][1]_out[58] sram[1671]->outb sram[1671]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[39], level=1, select_path_id=0. *****
*****1*****
Xsram[1671] sram->in sram[1671]->out sram[1671]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1671]->out) 0
.nodeset V(sram[1671]->outb) vsp
Xmux_1level_tapbuf_size2[40] grid[0][1]_pin[0][1][13] chanx[1][0]_in[63] chany[0][1]_out[60] sram[1672]->outb sram[1672]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[40], level=1, select_path_id=0. *****
*****1*****
Xsram[1672] sram->in sram[1672]->out sram[1672]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1672]->out) 0
.nodeset V(sram[1672]->outb) vsp
Xmux_1level_tapbuf_size2[41] grid[0][1]_pin[0][1][13] chanx[1][0]_in[65] chany[0][1]_out[62] sram[1673]->outb sram[1673]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[41], level=1, select_path_id=0. *****
*****1*****
Xsram[1673] sram->in sram[1673]->out sram[1673]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1673]->out) 0
.nodeset V(sram[1673]->outb) vsp
Xmux_1level_tapbuf_size2[42] grid[0][1]_pin[0][1][13] chanx[1][0]_in[67] chany[0][1]_out[64] sram[1674]->outb sram[1674]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[42], level=1, select_path_id=0. *****
*****1*****
Xsram[1674] sram->in sram[1674]->out sram[1674]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1674]->out) 0
.nodeset V(sram[1674]->outb) vsp
Xmux_1level_tapbuf_size2[43] grid[0][1]_pin[0][1][13] chanx[1][0]_in[69] chany[0][1]_out[66] sram[1675]->outb sram[1675]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[43], level=1, select_path_id=0. *****
*****1*****
Xsram[1675] sram->in sram[1675]->out sram[1675]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1675]->out) 0
.nodeset V(sram[1675]->outb) vsp
Xmux_1level_tapbuf_size2[44] grid[0][1]_pin[0][1][13] chanx[1][0]_in[71] chany[0][1]_out[68] sram[1676]->outb sram[1676]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[44], level=1, select_path_id=0. *****
*****1*****
Xsram[1676] sram->in sram[1676]->out sram[1676]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1676]->out) 0
.nodeset V(sram[1676]->outb) vsp
Xmux_1level_tapbuf_size2[45] grid[0][1]_pin[0][1][15] chanx[1][0]_in[73] chany[0][1]_out[70] sram[1677]->outb sram[1677]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[45], level=1, select_path_id=0. *****
*****1*****
Xsram[1677] sram->in sram[1677]->out sram[1677]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1677]->out) 0
.nodeset V(sram[1677]->outb) vsp
Xmux_1level_tapbuf_size2[46] grid[0][1]_pin[0][1][15] chanx[1][0]_in[75] chany[0][1]_out[72] sram[1678]->outb sram[1678]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[46], level=1, select_path_id=0. *****
*****1*****
Xsram[1678] sram->in sram[1678]->out sram[1678]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1678]->out) 0
.nodeset V(sram[1678]->outb) vsp
Xmux_1level_tapbuf_size2[47] grid[0][1]_pin[0][1][15] chanx[1][0]_in[77] chany[0][1]_out[74] sram[1679]->outb sram[1679]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[47], level=1, select_path_id=0. *****
*****1*****
Xsram[1679] sram->in sram[1679]->out sram[1679]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1679]->out) 0
.nodeset V(sram[1679]->outb) vsp
Xmux_1level_tapbuf_size2[48] grid[0][1]_pin[0][1][15] chanx[1][0]_in[79] chany[0][1]_out[76] sram[1680]->out sram[1680]->outb svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[48], level=1, select_path_id=1. *****
*****0*****
Xsram[1680] sram->in sram[1680]->out sram[1680]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1680]->out) 0
.nodeset V(sram[1680]->outb) vsp
Xmux_1level_tapbuf_size2[49] grid[0][1]_pin[0][1][15] chanx[1][0]_in[81] chany[0][1]_out[78] sram[1681]->outb sram[1681]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[49], level=1, select_path_id=0. *****
*****1*****
Xsram[1681] sram->in sram[1681]->out sram[1681]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1681]->out) 0
.nodeset V(sram[1681]->outb) vsp
Xmux_1level_tapbuf_size2[50] grid[1][1]_pin[0][3][43] chanx[1][0]_in[83] chany[0][1]_out[80] sram[1682]->outb sram[1682]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[50], level=1, select_path_id=0. *****
*****1*****
Xsram[1682] sram->in sram[1682]->out sram[1682]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1682]->out) 0
.nodeset V(sram[1682]->outb) vsp
Xmux_1level_tapbuf_size2[51] grid[1][1]_pin[0][3][43] chanx[1][0]_in[85] chany[0][1]_out[82] sram[1683]->outb sram[1683]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[51], level=1, select_path_id=0. *****
*****1*****
Xsram[1683] sram->in sram[1683]->out sram[1683]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1683]->out) 0
.nodeset V(sram[1683]->outb) vsp
Xmux_1level_tapbuf_size2[52] grid[1][1]_pin[0][3][43] chanx[1][0]_in[87] chany[0][1]_out[84] sram[1684]->outb sram[1684]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[52], level=1, select_path_id=0. *****
*****1*****
Xsram[1684] sram->in sram[1684]->out sram[1684]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1684]->out) 0
.nodeset V(sram[1684]->outb) vsp
Xmux_1level_tapbuf_size2[53] grid[1][1]_pin[0][3][43] chanx[1][0]_in[89] chany[0][1]_out[86] sram[1685]->outb sram[1685]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[53], level=1, select_path_id=0. *****
*****1*****
Xsram[1685] sram->in sram[1685]->out sram[1685]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1685]->out) 0
.nodeset V(sram[1685]->outb) vsp
Xmux_1level_tapbuf_size2[54] grid[1][1]_pin[0][3][43] chanx[1][0]_in[91] chany[0][1]_out[88] sram[1686]->outb sram[1686]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[54], level=1, select_path_id=0. *****
*****1*****
Xsram[1686] sram->in sram[1686]->out sram[1686]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1686]->out) 0
.nodeset V(sram[1686]->outb) vsp
Xmux_1level_tapbuf_size2[55] grid[1][1]_pin[0][3][47] chanx[1][0]_in[93] chany[0][1]_out[90] sram[1687]->outb sram[1687]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[55], level=1, select_path_id=0. *****
*****1*****
Xsram[1687] sram->in sram[1687]->out sram[1687]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1687]->out) 0
.nodeset V(sram[1687]->outb) vsp
Xmux_1level_tapbuf_size2[56] grid[1][1]_pin[0][3][47] chanx[1][0]_in[95] chany[0][1]_out[92] sram[1688]->outb sram[1688]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[56], level=1, select_path_id=0. *****
*****1*****
Xsram[1688] sram->in sram[1688]->out sram[1688]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1688]->out) 0
.nodeset V(sram[1688]->outb) vsp
Xmux_1level_tapbuf_size2[57] grid[1][1]_pin[0][3][47] chanx[1][0]_in[97] chany[0][1]_out[94] sram[1689]->outb sram[1689]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[57], level=1, select_path_id=0. *****
*****1*****
Xsram[1689] sram->in sram[1689]->out sram[1689]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1689]->out) 0
.nodeset V(sram[1689]->outb) vsp
Xmux_1level_tapbuf_size2[58] grid[1][1]_pin[0][3][47] chanx[1][0]_in[99] chany[0][1]_out[96] sram[1690]->outb sram[1690]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[58], level=1, select_path_id=0. *****
*****1*****
Xsram[1690] sram->in sram[1690]->out sram[1690]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1690]->out) 0
.nodeset V(sram[1690]->outb) vsp
Xmux_1level_tapbuf_size2[59] grid[1][1]_pin[0][3][47] chanx[1][0]_in[1] chany[0][1]_out[98] sram[1691]->outb sram[1691]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[59], level=1, select_path_id=0. *****
*****1*****
Xsram[1691] sram->in sram[1691]->out sram[1691]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1691]->out) 0
.nodeset V(sram[1691]->outb) vsp
***** right side Multiplexers *****
Xmux_1level_tapbuf_size2[60] grid[1][0]_pin[0][0][1] chany[0][1]_in[99] chanx[1][0]_out[0] sram[1692]->outb sram[1692]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[60], level=1, select_path_id=0. *****
*****1*****
Xsram[1692] sram->in sram[1692]->out sram[1692]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1692]->out) 0
.nodeset V(sram[1692]->outb) vsp
Xmux_1level_tapbuf_size2[61] grid[1][0]_pin[0][0][1] chany[0][1]_in[1] chanx[1][0]_out[2] sram[1693]->outb sram[1693]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[61], level=1, select_path_id=0. *****
*****1*****
Xsram[1693] sram->in sram[1693]->out sram[1693]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1693]->out) 0
.nodeset V(sram[1693]->outb) vsp
Xmux_1level_tapbuf_size2[62] grid[1][0]_pin[0][0][1] chany[0][1]_in[3] chanx[1][0]_out[4] sram[1694]->outb sram[1694]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[62], level=1, select_path_id=0. *****
*****1*****
Xsram[1694] sram->in sram[1694]->out sram[1694]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1694]->out) 0
.nodeset V(sram[1694]->outb) vsp
Xmux_1level_tapbuf_size2[63] grid[1][0]_pin[0][0][1] chany[0][1]_in[5] chanx[1][0]_out[6] sram[1695]->outb sram[1695]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[63], level=1, select_path_id=0. *****
*****1*****
Xsram[1695] sram->in sram[1695]->out sram[1695]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1695]->out) 0
.nodeset V(sram[1695]->outb) vsp
Xmux_1level_tapbuf_size2[64] grid[1][0]_pin[0][0][1] chany[0][1]_in[7] chanx[1][0]_out[8] sram[1696]->outb sram[1696]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[64], level=1, select_path_id=0. *****
*****1*****
Xsram[1696] sram->in sram[1696]->out sram[1696]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1696]->out) 0
.nodeset V(sram[1696]->outb) vsp
Xmux_1level_tapbuf_size2[65] grid[1][0]_pin[0][0][3] chany[0][1]_in[9] chanx[1][0]_out[10] sram[1697]->outb sram[1697]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[65], level=1, select_path_id=0. *****
*****1*****
Xsram[1697] sram->in sram[1697]->out sram[1697]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1697]->out) 0
.nodeset V(sram[1697]->outb) vsp
Xmux_1level_tapbuf_size2[66] grid[1][0]_pin[0][0][3] chany[0][1]_in[11] chanx[1][0]_out[12] sram[1698]->outb sram[1698]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[66], level=1, select_path_id=0. *****
*****1*****
Xsram[1698] sram->in sram[1698]->out sram[1698]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1698]->out) 0
.nodeset V(sram[1698]->outb) vsp
Xmux_1level_tapbuf_size2[67] grid[1][0]_pin[0][0][3] chany[0][1]_in[13] chanx[1][0]_out[14] sram[1699]->outb sram[1699]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[67], level=1, select_path_id=0. *****
*****1*****
Xsram[1699] sram->in sram[1699]->out sram[1699]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1699]->out) 0
.nodeset V(sram[1699]->outb) vsp
Xmux_1level_tapbuf_size2[68] grid[1][0]_pin[0][0][3] chany[0][1]_in[15] chanx[1][0]_out[16] sram[1700]->outb sram[1700]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[68], level=1, select_path_id=0. *****
*****1*****
Xsram[1700] sram->in sram[1700]->out sram[1700]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1700]->out) 0
.nodeset V(sram[1700]->outb) vsp
Xmux_1level_tapbuf_size2[69] grid[1][0]_pin[0][0][3] chany[0][1]_in[17] chanx[1][0]_out[18] sram[1701]->outb sram[1701]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[69], level=1, select_path_id=0. *****
*****1*****
Xsram[1701] sram->in sram[1701]->out sram[1701]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1701]->out) 0
.nodeset V(sram[1701]->outb) vsp
Xmux_1level_tapbuf_size2[70] grid[1][0]_pin[0][0][5] chany[0][1]_in[19] chanx[1][0]_out[20] sram[1702]->outb sram[1702]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[70], level=1, select_path_id=0. *****
*****1*****
Xsram[1702] sram->in sram[1702]->out sram[1702]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1702]->out) 0
.nodeset V(sram[1702]->outb) vsp
Xmux_1level_tapbuf_size2[71] grid[1][0]_pin[0][0][5] chany[0][1]_in[21] chanx[1][0]_out[22] sram[1703]->outb sram[1703]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[71], level=1, select_path_id=0. *****
*****1*****
Xsram[1703] sram->in sram[1703]->out sram[1703]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1703]->out) 0
.nodeset V(sram[1703]->outb) vsp
Xmux_1level_tapbuf_size2[72] grid[1][0]_pin[0][0][5] chany[0][1]_in[23] chanx[1][0]_out[24] sram[1704]->outb sram[1704]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[72], level=1, select_path_id=0. *****
*****1*****
Xsram[1704] sram->in sram[1704]->out sram[1704]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1704]->out) 0
.nodeset V(sram[1704]->outb) vsp
Xmux_1level_tapbuf_size2[73] grid[1][0]_pin[0][0][5] chany[0][1]_in[25] chanx[1][0]_out[26] sram[1705]->outb sram[1705]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[73], level=1, select_path_id=0. *****
*****1*****
Xsram[1705] sram->in sram[1705]->out sram[1705]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1705]->out) 0
.nodeset V(sram[1705]->outb) vsp
Xmux_1level_tapbuf_size2[74] grid[1][0]_pin[0][0][5] chany[0][1]_in[27] chanx[1][0]_out[28] sram[1706]->outb sram[1706]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[74], level=1, select_path_id=0. *****
*****1*****
Xsram[1706] sram->in sram[1706]->out sram[1706]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1706]->out) 0
.nodeset V(sram[1706]->outb) vsp
Xmux_1level_tapbuf_size2[75] grid[1][0]_pin[0][0][7] chany[0][1]_in[29] chanx[1][0]_out[30] sram[1707]->outb sram[1707]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[75], level=1, select_path_id=0. *****
*****1*****
Xsram[1707] sram->in sram[1707]->out sram[1707]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1707]->out) 0
.nodeset V(sram[1707]->outb) vsp
Xmux_1level_tapbuf_size2[76] grid[1][0]_pin[0][0][7] chany[0][1]_in[31] chanx[1][0]_out[32] sram[1708]->outb sram[1708]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[76], level=1, select_path_id=0. *****
*****1*****
Xsram[1708] sram->in sram[1708]->out sram[1708]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1708]->out) 0
.nodeset V(sram[1708]->outb) vsp
Xmux_1level_tapbuf_size2[77] grid[1][0]_pin[0][0][7] chany[0][1]_in[33] chanx[1][0]_out[34] sram[1709]->outb sram[1709]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[77], level=1, select_path_id=0. *****
*****1*****
Xsram[1709] sram->in sram[1709]->out sram[1709]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1709]->out) 0
.nodeset V(sram[1709]->outb) vsp
Xmux_1level_tapbuf_size2[78] grid[1][0]_pin[0][0][7] chany[0][1]_in[35] chanx[1][0]_out[36] sram[1710]->outb sram[1710]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[78], level=1, select_path_id=0. *****
*****1*****
Xsram[1710] sram->in sram[1710]->out sram[1710]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1710]->out) 0
.nodeset V(sram[1710]->outb) vsp
Xmux_1level_tapbuf_size2[79] grid[1][0]_pin[0][0][7] chany[0][1]_in[37] chanx[1][0]_out[38] sram[1711]->outb sram[1711]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[79], level=1, select_path_id=0. *****
*****1*****
Xsram[1711] sram->in sram[1711]->out sram[1711]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1711]->out) 0
.nodeset V(sram[1711]->outb) vsp
Xmux_1level_tapbuf_size2[80] grid[1][0]_pin[0][0][9] chany[0][1]_in[39] chanx[1][0]_out[40] sram[1712]->outb sram[1712]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[80], level=1, select_path_id=0. *****
*****1*****
Xsram[1712] sram->in sram[1712]->out sram[1712]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1712]->out) 0
.nodeset V(sram[1712]->outb) vsp
Xmux_1level_tapbuf_size2[81] grid[1][0]_pin[0][0][9] chany[0][1]_in[41] chanx[1][0]_out[42] sram[1713]->outb sram[1713]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[81], level=1, select_path_id=0. *****
*****1*****
Xsram[1713] sram->in sram[1713]->out sram[1713]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1713]->out) 0
.nodeset V(sram[1713]->outb) vsp
Xmux_1level_tapbuf_size2[82] grid[1][0]_pin[0][0][9] chany[0][1]_in[43] chanx[1][0]_out[44] sram[1714]->outb sram[1714]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[82], level=1, select_path_id=0. *****
*****1*****
Xsram[1714] sram->in sram[1714]->out sram[1714]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1714]->out) 0
.nodeset V(sram[1714]->outb) vsp
Xmux_1level_tapbuf_size2[83] grid[1][0]_pin[0][0][9] chany[0][1]_in[45] chanx[1][0]_out[46] sram[1715]->outb sram[1715]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[83], level=1, select_path_id=0. *****
*****1*****
Xsram[1715] sram->in sram[1715]->out sram[1715]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1715]->out) 0
.nodeset V(sram[1715]->outb) vsp
Xmux_1level_tapbuf_size2[84] grid[1][0]_pin[0][0][9] chany[0][1]_in[47] chanx[1][0]_out[48] sram[1716]->outb sram[1716]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[84], level=1, select_path_id=0. *****
*****1*****
Xsram[1716] sram->in sram[1716]->out sram[1716]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1716]->out) 0
.nodeset V(sram[1716]->outb) vsp
Xmux_1level_tapbuf_size2[85] grid[1][0]_pin[0][0][11] chany[0][1]_in[49] chanx[1][0]_out[50] sram[1717]->outb sram[1717]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[85], level=1, select_path_id=0. *****
*****1*****
Xsram[1717] sram->in sram[1717]->out sram[1717]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1717]->out) 0
.nodeset V(sram[1717]->outb) vsp
Xmux_1level_tapbuf_size2[86] grid[1][0]_pin[0][0][11] chany[0][1]_in[51] chanx[1][0]_out[52] sram[1718]->outb sram[1718]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[86], level=1, select_path_id=0. *****
*****1*****
Xsram[1718] sram->in sram[1718]->out sram[1718]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1718]->out) 0
.nodeset V(sram[1718]->outb) vsp
Xmux_1level_tapbuf_size2[87] grid[1][0]_pin[0][0][11] chany[0][1]_in[53] chanx[1][0]_out[54] sram[1719]->outb sram[1719]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[87], level=1, select_path_id=0. *****
*****1*****
Xsram[1719] sram->in sram[1719]->out sram[1719]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1719]->out) 0
.nodeset V(sram[1719]->outb) vsp
Xmux_1level_tapbuf_size2[88] grid[1][0]_pin[0][0][11] chany[0][1]_in[55] chanx[1][0]_out[56] sram[1720]->outb sram[1720]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[88], level=1, select_path_id=0. *****
*****1*****
Xsram[1720] sram->in sram[1720]->out sram[1720]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1720]->out) 0
.nodeset V(sram[1720]->outb) vsp
Xmux_1level_tapbuf_size2[89] grid[1][0]_pin[0][0][11] chany[0][1]_in[57] chanx[1][0]_out[58] sram[1721]->outb sram[1721]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[89], level=1, select_path_id=0. *****
*****1*****
Xsram[1721] sram->in sram[1721]->out sram[1721]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1721]->out) 0
.nodeset V(sram[1721]->outb) vsp
Xmux_1level_tapbuf_size2[90] grid[1][0]_pin[0][0][13] chany[0][1]_in[59] chanx[1][0]_out[60] sram[1722]->outb sram[1722]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[90], level=1, select_path_id=0. *****
*****1*****
Xsram[1722] sram->in sram[1722]->out sram[1722]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1722]->out) 0
.nodeset V(sram[1722]->outb) vsp
Xmux_1level_tapbuf_size2[91] grid[1][0]_pin[0][0][13] chany[0][1]_in[61] chanx[1][0]_out[62] sram[1723]->outb sram[1723]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[91], level=1, select_path_id=0. *****
*****1*****
Xsram[1723] sram->in sram[1723]->out sram[1723]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1723]->out) 0
.nodeset V(sram[1723]->outb) vsp
Xmux_1level_tapbuf_size2[92] grid[1][0]_pin[0][0][13] chany[0][1]_in[63] chanx[1][0]_out[64] sram[1724]->outb sram[1724]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[92], level=1, select_path_id=0. *****
*****1*****
Xsram[1724] sram->in sram[1724]->out sram[1724]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1724]->out) 0
.nodeset V(sram[1724]->outb) vsp
Xmux_1level_tapbuf_size2[93] grid[1][0]_pin[0][0][13] chany[0][1]_in[65] chanx[1][0]_out[66] sram[1725]->outb sram[1725]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[93], level=1, select_path_id=0. *****
*****1*****
Xsram[1725] sram->in sram[1725]->out sram[1725]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1725]->out) 0
.nodeset V(sram[1725]->outb) vsp
Xmux_1level_tapbuf_size2[94] grid[1][0]_pin[0][0][13] chany[0][1]_in[67] chanx[1][0]_out[68] sram[1726]->outb sram[1726]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[94], level=1, select_path_id=0. *****
*****1*****
Xsram[1726] sram->in sram[1726]->out sram[1726]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1726]->out) 0
.nodeset V(sram[1726]->outb) vsp
Xmux_1level_tapbuf_size2[95] grid[1][0]_pin[0][0][15] chany[0][1]_in[69] chanx[1][0]_out[70] sram[1727]->outb sram[1727]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[95], level=1, select_path_id=0. *****
*****1*****
Xsram[1727] sram->in sram[1727]->out sram[1727]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1727]->out) 0
.nodeset V(sram[1727]->outb) vsp
Xmux_1level_tapbuf_size2[96] grid[1][0]_pin[0][0][15] chany[0][1]_in[71] chanx[1][0]_out[72] sram[1728]->outb sram[1728]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[96], level=1, select_path_id=0. *****
*****1*****
Xsram[1728] sram->in sram[1728]->out sram[1728]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1728]->out) 0
.nodeset V(sram[1728]->outb) vsp
Xmux_1level_tapbuf_size2[97] grid[1][0]_pin[0][0][15] chany[0][1]_in[73] chanx[1][0]_out[74] sram[1729]->outb sram[1729]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[97], level=1, select_path_id=0. *****
*****1*****
Xsram[1729] sram->in sram[1729]->out sram[1729]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1729]->out) 0
.nodeset V(sram[1729]->outb) vsp
Xmux_1level_tapbuf_size2[98] grid[1][0]_pin[0][0][15] chany[0][1]_in[75] chanx[1][0]_out[76] sram[1730]->outb sram[1730]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[98], level=1, select_path_id=0. *****
*****1*****
Xsram[1730] sram->in sram[1730]->out sram[1730]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1730]->out) 0
.nodeset V(sram[1730]->outb) vsp
Xmux_1level_tapbuf_size2[99] grid[1][0]_pin[0][0][15] chany[0][1]_in[77] chanx[1][0]_out[78] sram[1731]->outb sram[1731]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[99], level=1, select_path_id=0. *****
*****1*****
Xsram[1731] sram->in sram[1731]->out sram[1731]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1731]->out) 0
.nodeset V(sram[1731]->outb) vsp
Xmux_1level_tapbuf_size2[100] grid[1][1]_pin[0][2][42] chany[0][1]_in[79] chanx[1][0]_out[80] sram[1732]->outb sram[1732]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[100], level=1, select_path_id=0. *****
*****1*****
Xsram[1732] sram->in sram[1732]->out sram[1732]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1732]->out) 0
.nodeset V(sram[1732]->outb) vsp
Xmux_1level_tapbuf_size2[101] grid[1][1]_pin[0][2][42] chany[0][1]_in[81] chanx[1][0]_out[82] sram[1733]->outb sram[1733]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[101], level=1, select_path_id=0. *****
*****1*****
Xsram[1733] sram->in sram[1733]->out sram[1733]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1733]->out) 0
.nodeset V(sram[1733]->outb) vsp
Xmux_1level_tapbuf_size2[102] grid[1][1]_pin[0][2][42] chany[0][1]_in[83] chanx[1][0]_out[84] sram[1734]->outb sram[1734]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[102], level=1, select_path_id=0. *****
*****1*****
Xsram[1734] sram->in sram[1734]->out sram[1734]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1734]->out) 0
.nodeset V(sram[1734]->outb) vsp
Xmux_1level_tapbuf_size2[103] grid[1][1]_pin[0][2][42] chany[0][1]_in[85] chanx[1][0]_out[86] sram[1735]->outb sram[1735]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[103], level=1, select_path_id=0. *****
*****1*****
Xsram[1735] sram->in sram[1735]->out sram[1735]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1735]->out) 0
.nodeset V(sram[1735]->outb) vsp
Xmux_1level_tapbuf_size2[104] grid[1][1]_pin[0][2][42] chany[0][1]_in[87] chanx[1][0]_out[88] sram[1736]->outb sram[1736]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[104], level=1, select_path_id=0. *****
*****1*****
Xsram[1736] sram->in sram[1736]->out sram[1736]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1736]->out) 0
.nodeset V(sram[1736]->outb) vsp
Xmux_1level_tapbuf_size2[105] grid[1][1]_pin[0][2][46] chany[0][1]_in[89] chanx[1][0]_out[90] sram[1737]->outb sram[1737]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[105], level=1, select_path_id=0. *****
*****1*****
Xsram[1737] sram->in sram[1737]->out sram[1737]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1737]->out) 0
.nodeset V(sram[1737]->outb) vsp
Xmux_1level_tapbuf_size2[106] grid[1][1]_pin[0][2][46] chany[0][1]_in[91] chanx[1][0]_out[92] sram[1738]->outb sram[1738]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[106], level=1, select_path_id=0. *****
*****1*****
Xsram[1738] sram->in sram[1738]->out sram[1738]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1738]->out) 0
.nodeset V(sram[1738]->outb) vsp
Xmux_1level_tapbuf_size2[107] grid[1][1]_pin[0][2][46] chany[0][1]_in[93] chanx[1][0]_out[94] sram[1739]->outb sram[1739]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[107], level=1, select_path_id=0. *****
*****1*****
Xsram[1739] sram->in sram[1739]->out sram[1739]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1739]->out) 0
.nodeset V(sram[1739]->outb) vsp
Xmux_1level_tapbuf_size2[108] grid[1][1]_pin[0][2][46] chany[0][1]_in[95] chanx[1][0]_out[96] sram[1740]->outb sram[1740]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[108], level=1, select_path_id=0. *****
*****1*****
Xsram[1740] sram->in sram[1740]->out sram[1740]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1740]->out) 0
.nodeset V(sram[1740]->outb) vsp
Xmux_1level_tapbuf_size2[109] grid[1][1]_pin[0][2][46] chany[0][1]_in[97] chanx[1][0]_out[98] sram[1741]->outb sram[1741]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[109], level=1, select_path_id=0. *****
*****1*****
Xsram[1741] sram->in sram[1741]->out sram[1741]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1741]->out) 0
.nodeset V(sram[1741]->outb) vsp
***** bottom side Multiplexers *****
***** left side Multiplexers *****
.eom
