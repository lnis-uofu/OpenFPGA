*****************************
*     FPGA SPICE Netlist    *
* Description: Switch Block  [0][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
***** Switch Box[0][1] Sub-Circuit *****
.subckt sb[0][1] 
***** Inputs/outputs of top side *****
+ 
+ 
+ ***** Inputs/outputs of right side *****
+ chanx[1][1]_out[0] chanx[1][1]_in[1] chanx[1][1]_out[2] chanx[1][1]_in[3] chanx[1][1]_out[4] chanx[1][1]_in[5] chanx[1][1]_out[6] chanx[1][1]_in[7] chanx[1][1]_out[8] chanx[1][1]_in[9] chanx[1][1]_out[10] chanx[1][1]_in[11] chanx[1][1]_out[12] chanx[1][1]_in[13] chanx[1][1]_out[14] chanx[1][1]_in[15] chanx[1][1]_out[16] chanx[1][1]_in[17] chanx[1][1]_out[18] chanx[1][1]_in[19] chanx[1][1]_out[20] chanx[1][1]_in[21] chanx[1][1]_out[22] chanx[1][1]_in[23] chanx[1][1]_out[24] chanx[1][1]_in[25] chanx[1][1]_out[26] chanx[1][1]_in[27] chanx[1][1]_out[28] chanx[1][1]_in[29] chanx[1][1]_out[30] chanx[1][1]_in[31] chanx[1][1]_out[32] chanx[1][1]_in[33] chanx[1][1]_out[34] chanx[1][1]_in[35] chanx[1][1]_out[36] chanx[1][1]_in[37] chanx[1][1]_out[38] chanx[1][1]_in[39] chanx[1][1]_out[40] chanx[1][1]_in[41] chanx[1][1]_out[42] chanx[1][1]_in[43] chanx[1][1]_out[44] chanx[1][1]_in[45] chanx[1][1]_out[46] chanx[1][1]_in[47] chanx[1][1]_out[48] chanx[1][1]_in[49] chanx[1][1]_out[50] chanx[1][1]_in[51] chanx[1][1]_out[52] chanx[1][1]_in[53] chanx[1][1]_out[54] chanx[1][1]_in[55] chanx[1][1]_out[56] chanx[1][1]_in[57] chanx[1][1]_out[58] chanx[1][1]_in[59] chanx[1][1]_out[60] chanx[1][1]_in[61] chanx[1][1]_out[62] chanx[1][1]_in[63] chanx[1][1]_out[64] chanx[1][1]_in[65] chanx[1][1]_out[66] chanx[1][1]_in[67] chanx[1][1]_out[68] chanx[1][1]_in[69] chanx[1][1]_out[70] chanx[1][1]_in[71] chanx[1][1]_out[72] chanx[1][1]_in[73] chanx[1][1]_out[74] chanx[1][1]_in[75] chanx[1][1]_out[76] chanx[1][1]_in[77] chanx[1][1]_out[78] chanx[1][1]_in[79] chanx[1][1]_out[80] chanx[1][1]_in[81] chanx[1][1]_out[82] chanx[1][1]_in[83] chanx[1][1]_out[84] chanx[1][1]_in[85] chanx[1][1]_out[86] chanx[1][1]_in[87] chanx[1][1]_out[88] chanx[1][1]_in[89] chanx[1][1]_out[90] chanx[1][1]_in[91] chanx[1][1]_out[92] chanx[1][1]_in[93] chanx[1][1]_out[94] chanx[1][1]_in[95] chanx[1][1]_out[96] chanx[1][1]_in[97] chanx[1][1]_out[98] chanx[1][1]_in[99] 
+ grid[1][2]_pin[0][2][1] grid[1][2]_pin[0][2][3] grid[1][2]_pin[0][2][5] grid[1][2]_pin[0][2][7] grid[1][2]_pin[0][2][9] grid[1][2]_pin[0][2][11] grid[1][2]_pin[0][2][13] grid[1][2]_pin[0][2][15] grid[1][1]_pin[0][0][40] grid[1][1]_pin[0][0][44] grid[1][1]_pin[0][0][48] 
+ ***** Inputs/outputs of bottom side *****
+ chany[0][1]_in[0] chany[0][1]_out[1] chany[0][1]_in[2] chany[0][1]_out[3] chany[0][1]_in[4] chany[0][1]_out[5] chany[0][1]_in[6] chany[0][1]_out[7] chany[0][1]_in[8] chany[0][1]_out[9] chany[0][1]_in[10] chany[0][1]_out[11] chany[0][1]_in[12] chany[0][1]_out[13] chany[0][1]_in[14] chany[0][1]_out[15] chany[0][1]_in[16] chany[0][1]_out[17] chany[0][1]_in[18] chany[0][1]_out[19] chany[0][1]_in[20] chany[0][1]_out[21] chany[0][1]_in[22] chany[0][1]_out[23] chany[0][1]_in[24] chany[0][1]_out[25] chany[0][1]_in[26] chany[0][1]_out[27] chany[0][1]_in[28] chany[0][1]_out[29] chany[0][1]_in[30] chany[0][1]_out[31] chany[0][1]_in[32] chany[0][1]_out[33] chany[0][1]_in[34] chany[0][1]_out[35] chany[0][1]_in[36] chany[0][1]_out[37] chany[0][1]_in[38] chany[0][1]_out[39] chany[0][1]_in[40] chany[0][1]_out[41] chany[0][1]_in[42] chany[0][1]_out[43] chany[0][1]_in[44] chany[0][1]_out[45] chany[0][1]_in[46] chany[0][1]_out[47] chany[0][1]_in[48] chany[0][1]_out[49] chany[0][1]_in[50] chany[0][1]_out[51] chany[0][1]_in[52] chany[0][1]_out[53] chany[0][1]_in[54] chany[0][1]_out[55] chany[0][1]_in[56] chany[0][1]_out[57] chany[0][1]_in[58] chany[0][1]_out[59] chany[0][1]_in[60] chany[0][1]_out[61] chany[0][1]_in[62] chany[0][1]_out[63] chany[0][1]_in[64] chany[0][1]_out[65] chany[0][1]_in[66] chany[0][1]_out[67] chany[0][1]_in[68] chany[0][1]_out[69] chany[0][1]_in[70] chany[0][1]_out[71] chany[0][1]_in[72] chany[0][1]_out[73] chany[0][1]_in[74] chany[0][1]_out[75] chany[0][1]_in[76] chany[0][1]_out[77] chany[0][1]_in[78] chany[0][1]_out[79] chany[0][1]_in[80] chany[0][1]_out[81] chany[0][1]_in[82] chany[0][1]_out[83] chany[0][1]_in[84] chany[0][1]_out[85] chany[0][1]_in[86] chany[0][1]_out[87] chany[0][1]_in[88] chany[0][1]_out[89] chany[0][1]_in[90] chany[0][1]_out[91] chany[0][1]_in[92] chany[0][1]_out[93] chany[0][1]_in[94] chany[0][1]_out[95] chany[0][1]_in[96] chany[0][1]_out[97] chany[0][1]_in[98] chany[0][1]_out[99] 
+ grid[1][1]_pin[0][3][43] grid[1][1]_pin[0][3][47] grid[0][1]_pin[0][1][1] grid[0][1]_pin[0][1][3] grid[0][1]_pin[0][1][5] grid[0][1]_pin[0][1][7] grid[0][1]_pin[0][1][9] grid[0][1]_pin[0][1][11] grid[0][1]_pin[0][1][13] grid[0][1]_pin[0][1][15] 
+ ***** Inputs/outputs of left side *****
+ 
+ 
+ svdd sgnd
***** top side Multiplexers *****
***** right side Multiplexers *****
Xmux_1level_tapbuf_size3[110] grid[1][1]_pin[0][0][40] grid[1][2]_pin[0][2][15] chany[0][1]_in[96] chanx[1][1]_out[0] sram[1742]->outb sram[1742]->out sram[1743]->out sram[1743]->outb sram[1744]->out sram[1744]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[110], level=1, select_path_id=0. *****
*****100*****
Xsram[1742] sram->in sram[1742]->out sram[1742]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1742]->out) 0
.nodeset V(sram[1742]->outb) vsp
Xsram[1743] sram->in sram[1743]->out sram[1743]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1743]->out) 0
.nodeset V(sram[1743]->outb) vsp
Xsram[1744] sram->in sram[1744]->out sram[1744]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1744]->out) 0
.nodeset V(sram[1744]->outb) vsp
Xmux_1level_tapbuf_size3[111] grid[1][1]_pin[0][0][40] grid[1][2]_pin[0][2][15] chany[0][1]_in[94] chanx[1][1]_out[2] sram[1745]->outb sram[1745]->out sram[1746]->out sram[1746]->outb sram[1747]->out sram[1747]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[111], level=1, select_path_id=0. *****
*****100*****
Xsram[1745] sram->in sram[1745]->out sram[1745]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1745]->out) 0
.nodeset V(sram[1745]->outb) vsp
Xsram[1746] sram->in sram[1746]->out sram[1746]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1746]->out) 0
.nodeset V(sram[1746]->outb) vsp
Xsram[1747] sram->in sram[1747]->out sram[1747]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1747]->out) 0
.nodeset V(sram[1747]->outb) vsp
Xmux_1level_tapbuf_size3[112] grid[1][1]_pin[0][0][40] grid[1][2]_pin[0][2][15] chany[0][1]_in[92] chanx[1][1]_out[4] sram[1748]->outb sram[1748]->out sram[1749]->out sram[1749]->outb sram[1750]->out sram[1750]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[112], level=1, select_path_id=0. *****
*****100*****
Xsram[1748] sram->in sram[1748]->out sram[1748]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1748]->out) 0
.nodeset V(sram[1748]->outb) vsp
Xsram[1749] sram->in sram[1749]->out sram[1749]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1749]->out) 0
.nodeset V(sram[1749]->outb) vsp
Xsram[1750] sram->in sram[1750]->out sram[1750]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1750]->out) 0
.nodeset V(sram[1750]->outb) vsp
Xmux_1level_tapbuf_size3[113] grid[1][1]_pin[0][0][40] grid[1][2]_pin[0][2][15] chany[0][1]_in[90] chanx[1][1]_out[6] sram[1751]->outb sram[1751]->out sram[1752]->out sram[1752]->outb sram[1753]->out sram[1753]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[113], level=1, select_path_id=0. *****
*****100*****
Xsram[1751] sram->in sram[1751]->out sram[1751]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1751]->out) 0
.nodeset V(sram[1751]->outb) vsp
Xsram[1752] sram->in sram[1752]->out sram[1752]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1752]->out) 0
.nodeset V(sram[1752]->outb) vsp
Xsram[1753] sram->in sram[1753]->out sram[1753]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1753]->out) 0
.nodeset V(sram[1753]->outb) vsp
Xmux_1level_tapbuf_size3[114] grid[1][1]_pin[0][0][40] grid[1][2]_pin[0][2][15] chany[0][1]_in[88] chanx[1][1]_out[8] sram[1754]->outb sram[1754]->out sram[1755]->out sram[1755]->outb sram[1756]->out sram[1756]->outb svdd sgnd mux_1level_tapbuf_size3
***** SRAM bits for MUX[114], level=1, select_path_id=0. *****
*****100*****
Xsram[1754] sram->in sram[1754]->out sram[1754]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1754]->out) 0
.nodeset V(sram[1754]->outb) vsp
Xsram[1755] sram->in sram[1755]->out sram[1755]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1755]->out) 0
.nodeset V(sram[1755]->outb) vsp
Xsram[1756] sram->in sram[1756]->out sram[1756]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1756]->out) 0
.nodeset V(sram[1756]->outb) vsp
Xmux_1level_tapbuf_size2[115] grid[1][1]_pin[0][0][44] chany[0][1]_in[86] chanx[1][1]_out[10] sram[1757]->outb sram[1757]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[115], level=1, select_path_id=0. *****
*****1*****
Xsram[1757] sram->in sram[1757]->out sram[1757]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1757]->out) 0
.nodeset V(sram[1757]->outb) vsp
Xmux_1level_tapbuf_size2[116] grid[1][1]_pin[0][0][44] chany[0][1]_in[84] chanx[1][1]_out[12] sram[1758]->outb sram[1758]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[116], level=1, select_path_id=0. *****
*****1*****
Xsram[1758] sram->in sram[1758]->out sram[1758]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1758]->out) 0
.nodeset V(sram[1758]->outb) vsp
Xmux_1level_tapbuf_size2[117] grid[1][1]_pin[0][0][44] chany[0][1]_in[82] chanx[1][1]_out[14] sram[1759]->outb sram[1759]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[117], level=1, select_path_id=0. *****
*****1*****
Xsram[1759] sram->in sram[1759]->out sram[1759]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1759]->out) 0
.nodeset V(sram[1759]->outb) vsp
Xmux_1level_tapbuf_size2[118] grid[1][1]_pin[0][0][44] chany[0][1]_in[80] chanx[1][1]_out[16] sram[1760]->outb sram[1760]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[118], level=1, select_path_id=0. *****
*****1*****
Xsram[1760] sram->in sram[1760]->out sram[1760]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1760]->out) 0
.nodeset V(sram[1760]->outb) vsp
Xmux_1level_tapbuf_size2[119] grid[1][1]_pin[0][0][44] chany[0][1]_in[78] chanx[1][1]_out[18] sram[1761]->outb sram[1761]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[119], level=1, select_path_id=0. *****
*****1*****
Xsram[1761] sram->in sram[1761]->out sram[1761]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1761]->out) 0
.nodeset V(sram[1761]->outb) vsp
Xmux_1level_tapbuf_size2[120] grid[1][1]_pin[0][0][48] chany[0][1]_in[76] chanx[1][1]_out[20] sram[1762]->out sram[1762]->outb svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[120], level=1, select_path_id=1. *****
*****0*****
Xsram[1762] sram->in sram[1762]->out sram[1762]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1762]->out) 0
.nodeset V(sram[1762]->outb) vsp
Xmux_1level_tapbuf_size2[121] grid[1][1]_pin[0][0][48] chany[0][1]_in[74] chanx[1][1]_out[22] sram[1763]->outb sram[1763]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[121], level=1, select_path_id=0. *****
*****1*****
Xsram[1763] sram->in sram[1763]->out sram[1763]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1763]->out) 0
.nodeset V(sram[1763]->outb) vsp
Xmux_1level_tapbuf_size2[122] grid[1][1]_pin[0][0][48] chany[0][1]_in[72] chanx[1][1]_out[24] sram[1764]->outb sram[1764]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[122], level=1, select_path_id=0. *****
*****1*****
Xsram[1764] sram->in sram[1764]->out sram[1764]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1764]->out) 0
.nodeset V(sram[1764]->outb) vsp
Xmux_1level_tapbuf_size2[123] grid[1][1]_pin[0][0][48] chany[0][1]_in[70] chanx[1][1]_out[26] sram[1765]->outb sram[1765]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[123], level=1, select_path_id=0. *****
*****1*****
Xsram[1765] sram->in sram[1765]->out sram[1765]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1765]->out) 0
.nodeset V(sram[1765]->outb) vsp
Xmux_1level_tapbuf_size2[124] grid[1][1]_pin[0][0][48] chany[0][1]_in[68] chanx[1][1]_out[28] sram[1766]->outb sram[1766]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[124], level=1, select_path_id=0. *****
*****1*****
Xsram[1766] sram->in sram[1766]->out sram[1766]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1766]->out) 0
.nodeset V(sram[1766]->outb) vsp
Xmux_1level_tapbuf_size2[125] grid[1][2]_pin[0][2][1] chany[0][1]_in[66] chanx[1][1]_out[30] sram[1767]->outb sram[1767]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[125], level=1, select_path_id=0. *****
*****1*****
Xsram[1767] sram->in sram[1767]->out sram[1767]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1767]->out) 0
.nodeset V(sram[1767]->outb) vsp
Xmux_1level_tapbuf_size2[126] grid[1][2]_pin[0][2][1] chany[0][1]_in[64] chanx[1][1]_out[32] sram[1768]->outb sram[1768]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[126], level=1, select_path_id=0. *****
*****1*****
Xsram[1768] sram->in sram[1768]->out sram[1768]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1768]->out) 0
.nodeset V(sram[1768]->outb) vsp
Xmux_1level_tapbuf_size2[127] grid[1][2]_pin[0][2][1] chany[0][1]_in[62] chanx[1][1]_out[34] sram[1769]->outb sram[1769]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[127], level=1, select_path_id=0. *****
*****1*****
Xsram[1769] sram->in sram[1769]->out sram[1769]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1769]->out) 0
.nodeset V(sram[1769]->outb) vsp
Xmux_1level_tapbuf_size2[128] grid[1][2]_pin[0][2][1] chany[0][1]_in[60] chanx[1][1]_out[36] sram[1770]->outb sram[1770]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[128], level=1, select_path_id=0. *****
*****1*****
Xsram[1770] sram->in sram[1770]->out sram[1770]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1770]->out) 0
.nodeset V(sram[1770]->outb) vsp
Xmux_1level_tapbuf_size2[129] grid[1][2]_pin[0][2][1] chany[0][1]_in[58] chanx[1][1]_out[38] sram[1771]->outb sram[1771]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[129], level=1, select_path_id=0. *****
*****1*****
Xsram[1771] sram->in sram[1771]->out sram[1771]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1771]->out) 0
.nodeset V(sram[1771]->outb) vsp
Xmux_1level_tapbuf_size2[130] grid[1][2]_pin[0][2][3] chany[0][1]_in[56] chanx[1][1]_out[40] sram[1772]->outb sram[1772]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[130], level=1, select_path_id=0. *****
*****1*****
Xsram[1772] sram->in sram[1772]->out sram[1772]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1772]->out) 0
.nodeset V(sram[1772]->outb) vsp
Xmux_1level_tapbuf_size2[131] grid[1][2]_pin[0][2][3] chany[0][1]_in[54] chanx[1][1]_out[42] sram[1773]->outb sram[1773]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[131], level=1, select_path_id=0. *****
*****1*****
Xsram[1773] sram->in sram[1773]->out sram[1773]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1773]->out) 0
.nodeset V(sram[1773]->outb) vsp
Xmux_1level_tapbuf_size2[132] grid[1][2]_pin[0][2][3] chany[0][1]_in[52] chanx[1][1]_out[44] sram[1774]->outb sram[1774]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[132], level=1, select_path_id=0. *****
*****1*****
Xsram[1774] sram->in sram[1774]->out sram[1774]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1774]->out) 0
.nodeset V(sram[1774]->outb) vsp
Xmux_1level_tapbuf_size2[133] grid[1][2]_pin[0][2][3] chany[0][1]_in[50] chanx[1][1]_out[46] sram[1775]->outb sram[1775]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[133], level=1, select_path_id=0. *****
*****1*****
Xsram[1775] sram->in sram[1775]->out sram[1775]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1775]->out) 0
.nodeset V(sram[1775]->outb) vsp
Xmux_1level_tapbuf_size2[134] grid[1][2]_pin[0][2][3] chany[0][1]_in[48] chanx[1][1]_out[48] sram[1776]->outb sram[1776]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[134], level=1, select_path_id=0. *****
*****1*****
Xsram[1776] sram->in sram[1776]->out sram[1776]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1776]->out) 0
.nodeset V(sram[1776]->outb) vsp
Xmux_1level_tapbuf_size2[135] grid[1][2]_pin[0][2][5] chany[0][1]_in[46] chanx[1][1]_out[50] sram[1777]->outb sram[1777]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[135], level=1, select_path_id=0. *****
*****1*****
Xsram[1777] sram->in sram[1777]->out sram[1777]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1777]->out) 0
.nodeset V(sram[1777]->outb) vsp
Xmux_1level_tapbuf_size2[136] grid[1][2]_pin[0][2][5] chany[0][1]_in[44] chanx[1][1]_out[52] sram[1778]->outb sram[1778]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[136], level=1, select_path_id=0. *****
*****1*****
Xsram[1778] sram->in sram[1778]->out sram[1778]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1778]->out) 0
.nodeset V(sram[1778]->outb) vsp
Xmux_1level_tapbuf_size2[137] grid[1][2]_pin[0][2][5] chany[0][1]_in[42] chanx[1][1]_out[54] sram[1779]->outb sram[1779]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[137], level=1, select_path_id=0. *****
*****1*****
Xsram[1779] sram->in sram[1779]->out sram[1779]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1779]->out) 0
.nodeset V(sram[1779]->outb) vsp
Xmux_1level_tapbuf_size2[138] grid[1][2]_pin[0][2][5] chany[0][1]_in[40] chanx[1][1]_out[56] sram[1780]->outb sram[1780]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[138], level=1, select_path_id=0. *****
*****1*****
Xsram[1780] sram->in sram[1780]->out sram[1780]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1780]->out) 0
.nodeset V(sram[1780]->outb) vsp
Xmux_1level_tapbuf_size2[139] grid[1][2]_pin[0][2][5] chany[0][1]_in[38] chanx[1][1]_out[58] sram[1781]->outb sram[1781]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[139], level=1, select_path_id=0. *****
*****1*****
Xsram[1781] sram->in sram[1781]->out sram[1781]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1781]->out) 0
.nodeset V(sram[1781]->outb) vsp
Xmux_1level_tapbuf_size2[140] grid[1][2]_pin[0][2][7] chany[0][1]_in[36] chanx[1][1]_out[60] sram[1782]->outb sram[1782]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[140], level=1, select_path_id=0. *****
*****1*****
Xsram[1782] sram->in sram[1782]->out sram[1782]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1782]->out) 0
.nodeset V(sram[1782]->outb) vsp
Xmux_1level_tapbuf_size2[141] grid[1][2]_pin[0][2][7] chany[0][1]_in[34] chanx[1][1]_out[62] sram[1783]->outb sram[1783]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[141], level=1, select_path_id=0. *****
*****1*****
Xsram[1783] sram->in sram[1783]->out sram[1783]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1783]->out) 0
.nodeset V(sram[1783]->outb) vsp
Xmux_1level_tapbuf_size2[142] grid[1][2]_pin[0][2][7] chany[0][1]_in[32] chanx[1][1]_out[64] sram[1784]->outb sram[1784]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[142], level=1, select_path_id=0. *****
*****1*****
Xsram[1784] sram->in sram[1784]->out sram[1784]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1784]->out) 0
.nodeset V(sram[1784]->outb) vsp
Xmux_1level_tapbuf_size2[143] grid[1][2]_pin[0][2][7] chany[0][1]_in[30] chanx[1][1]_out[66] sram[1785]->outb sram[1785]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[143], level=1, select_path_id=0. *****
*****1*****
Xsram[1785] sram->in sram[1785]->out sram[1785]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1785]->out) 0
.nodeset V(sram[1785]->outb) vsp
Xmux_1level_tapbuf_size2[144] grid[1][2]_pin[0][2][7] chany[0][1]_in[28] chanx[1][1]_out[68] sram[1786]->outb sram[1786]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[144], level=1, select_path_id=0. *****
*****1*****
Xsram[1786] sram->in sram[1786]->out sram[1786]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1786]->out) 0
.nodeset V(sram[1786]->outb) vsp
Xmux_1level_tapbuf_size2[145] grid[1][2]_pin[0][2][9] chany[0][1]_in[26] chanx[1][1]_out[70] sram[1787]->outb sram[1787]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[145], level=1, select_path_id=0. *****
*****1*****
Xsram[1787] sram->in sram[1787]->out sram[1787]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1787]->out) 0
.nodeset V(sram[1787]->outb) vsp
Xmux_1level_tapbuf_size2[146] grid[1][2]_pin[0][2][9] chany[0][1]_in[24] chanx[1][1]_out[72] sram[1788]->outb sram[1788]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[146], level=1, select_path_id=0. *****
*****1*****
Xsram[1788] sram->in sram[1788]->out sram[1788]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1788]->out) 0
.nodeset V(sram[1788]->outb) vsp
Xmux_1level_tapbuf_size2[147] grid[1][2]_pin[0][2][9] chany[0][1]_in[22] chanx[1][1]_out[74] sram[1789]->outb sram[1789]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[147], level=1, select_path_id=0. *****
*****1*****
Xsram[1789] sram->in sram[1789]->out sram[1789]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1789]->out) 0
.nodeset V(sram[1789]->outb) vsp
Xmux_1level_tapbuf_size2[148] grid[1][2]_pin[0][2][9] chany[0][1]_in[20] chanx[1][1]_out[76] sram[1790]->outb sram[1790]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[148], level=1, select_path_id=0. *****
*****1*****
Xsram[1790] sram->in sram[1790]->out sram[1790]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1790]->out) 0
.nodeset V(sram[1790]->outb) vsp
Xmux_1level_tapbuf_size2[149] grid[1][2]_pin[0][2][9] chany[0][1]_in[18] chanx[1][1]_out[78] sram[1791]->outb sram[1791]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[149], level=1, select_path_id=0. *****
*****1*****
Xsram[1791] sram->in sram[1791]->out sram[1791]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1791]->out) 0
.nodeset V(sram[1791]->outb) vsp
Xmux_1level_tapbuf_size2[150] grid[1][2]_pin[0][2][11] chany[0][1]_in[16] chanx[1][1]_out[80] sram[1792]->outb sram[1792]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[150], level=1, select_path_id=0. *****
*****1*****
Xsram[1792] sram->in sram[1792]->out sram[1792]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1792]->out) 0
.nodeset V(sram[1792]->outb) vsp
Xmux_1level_tapbuf_size2[151] grid[1][2]_pin[0][2][11] chany[0][1]_in[14] chanx[1][1]_out[82] sram[1793]->outb sram[1793]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[151], level=1, select_path_id=0. *****
*****1*****
Xsram[1793] sram->in sram[1793]->out sram[1793]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1793]->out) 0
.nodeset V(sram[1793]->outb) vsp
Xmux_1level_tapbuf_size2[152] grid[1][2]_pin[0][2][11] chany[0][1]_in[12] chanx[1][1]_out[84] sram[1794]->outb sram[1794]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[152], level=1, select_path_id=0. *****
*****1*****
Xsram[1794] sram->in sram[1794]->out sram[1794]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1794]->out) 0
.nodeset V(sram[1794]->outb) vsp
Xmux_1level_tapbuf_size2[153] grid[1][2]_pin[0][2][11] chany[0][1]_in[10] chanx[1][1]_out[86] sram[1795]->outb sram[1795]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[153], level=1, select_path_id=0. *****
*****1*****
Xsram[1795] sram->in sram[1795]->out sram[1795]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1795]->out) 0
.nodeset V(sram[1795]->outb) vsp
Xmux_1level_tapbuf_size2[154] grid[1][2]_pin[0][2][11] chany[0][1]_in[8] chanx[1][1]_out[88] sram[1796]->outb sram[1796]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[154], level=1, select_path_id=0. *****
*****1*****
Xsram[1796] sram->in sram[1796]->out sram[1796]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1796]->out) 0
.nodeset V(sram[1796]->outb) vsp
Xmux_1level_tapbuf_size2[155] grid[1][2]_pin[0][2][13] chany[0][1]_in[6] chanx[1][1]_out[90] sram[1797]->outb sram[1797]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[155], level=1, select_path_id=0. *****
*****1*****
Xsram[1797] sram->in sram[1797]->out sram[1797]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1797]->out) 0
.nodeset V(sram[1797]->outb) vsp
Xmux_1level_tapbuf_size2[156] grid[1][2]_pin[0][2][13] chany[0][1]_in[4] chanx[1][1]_out[92] sram[1798]->outb sram[1798]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[156], level=1, select_path_id=0. *****
*****1*****
Xsram[1798] sram->in sram[1798]->out sram[1798]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1798]->out) 0
.nodeset V(sram[1798]->outb) vsp
Xmux_1level_tapbuf_size2[157] grid[1][2]_pin[0][2][13] chany[0][1]_in[2] chanx[1][1]_out[94] sram[1799]->outb sram[1799]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[157], level=1, select_path_id=0. *****
*****1*****
Xsram[1799] sram->in sram[1799]->out sram[1799]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1799]->out) 0
.nodeset V(sram[1799]->outb) vsp
Xmux_1level_tapbuf_size2[158] grid[1][2]_pin[0][2][13] chany[0][1]_in[0] chanx[1][1]_out[96] sram[1800]->outb sram[1800]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[158], level=1, select_path_id=0. *****
*****1*****
Xsram[1800] sram->in sram[1800]->out sram[1800]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1800]->out) 0
.nodeset V(sram[1800]->outb) vsp
Xmux_1level_tapbuf_size2[159] grid[1][2]_pin[0][2][13] chany[0][1]_in[98] chanx[1][1]_out[98] sram[1801]->outb sram[1801]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[159], level=1, select_path_id=0. *****
*****1*****
Xsram[1801] sram->in sram[1801]->out sram[1801]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1801]->out) 0
.nodeset V(sram[1801]->outb) vsp
***** bottom side Multiplexers *****
Xmux_1level_tapbuf_size2[160] grid[0][1]_pin[0][1][1] chanx[1][1]_in[97] chany[0][1]_out[1] sram[1802]->outb sram[1802]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[160], level=1, select_path_id=0. *****
*****1*****
Xsram[1802] sram->in sram[1802]->out sram[1802]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1802]->out) 0
.nodeset V(sram[1802]->outb) vsp
Xmux_1level_tapbuf_size2[161] grid[0][1]_pin[0][1][1] chanx[1][1]_in[95] chany[0][1]_out[3] sram[1803]->outb sram[1803]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[161], level=1, select_path_id=0. *****
*****1*****
Xsram[1803] sram->in sram[1803]->out sram[1803]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1803]->out) 0
.nodeset V(sram[1803]->outb) vsp
Xmux_1level_tapbuf_size2[162] grid[0][1]_pin[0][1][1] chanx[1][1]_in[93] chany[0][1]_out[5] sram[1804]->outb sram[1804]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[162], level=1, select_path_id=0. *****
*****1*****
Xsram[1804] sram->in sram[1804]->out sram[1804]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1804]->out) 0
.nodeset V(sram[1804]->outb) vsp
Xmux_1level_tapbuf_size2[163] grid[0][1]_pin[0][1][1] chanx[1][1]_in[91] chany[0][1]_out[7] sram[1805]->outb sram[1805]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[163], level=1, select_path_id=0. *****
*****1*****
Xsram[1805] sram->in sram[1805]->out sram[1805]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1805]->out) 0
.nodeset V(sram[1805]->outb) vsp
Xmux_1level_tapbuf_size2[164] grid[0][1]_pin[0][1][1] chanx[1][1]_in[89] chany[0][1]_out[9] sram[1806]->outb sram[1806]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[164], level=1, select_path_id=0. *****
*****1*****
Xsram[1806] sram->in sram[1806]->out sram[1806]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1806]->out) 0
.nodeset V(sram[1806]->outb) vsp
Xmux_1level_tapbuf_size2[165] grid[0][1]_pin[0][1][3] chanx[1][1]_in[87] chany[0][1]_out[11] sram[1807]->outb sram[1807]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[165], level=1, select_path_id=0. *****
*****1*****
Xsram[1807] sram->in sram[1807]->out sram[1807]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1807]->out) 0
.nodeset V(sram[1807]->outb) vsp
Xmux_1level_tapbuf_size2[166] grid[0][1]_pin[0][1][3] chanx[1][1]_in[85] chany[0][1]_out[13] sram[1808]->outb sram[1808]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[166], level=1, select_path_id=0. *****
*****1*****
Xsram[1808] sram->in sram[1808]->out sram[1808]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1808]->out) 0
.nodeset V(sram[1808]->outb) vsp
Xmux_1level_tapbuf_size2[167] grid[0][1]_pin[0][1][3] chanx[1][1]_in[83] chany[0][1]_out[15] sram[1809]->outb sram[1809]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[167], level=1, select_path_id=0. *****
*****1*****
Xsram[1809] sram->in sram[1809]->out sram[1809]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1809]->out) 0
.nodeset V(sram[1809]->outb) vsp
Xmux_1level_tapbuf_size2[168] grid[0][1]_pin[0][1][3] chanx[1][1]_in[81] chany[0][1]_out[17] sram[1810]->outb sram[1810]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[168], level=1, select_path_id=0. *****
*****1*****
Xsram[1810] sram->in sram[1810]->out sram[1810]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1810]->out) 0
.nodeset V(sram[1810]->outb) vsp
Xmux_1level_tapbuf_size2[169] grid[0][1]_pin[0][1][3] chanx[1][1]_in[79] chany[0][1]_out[19] sram[1811]->outb sram[1811]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[169], level=1, select_path_id=0. *****
*****1*****
Xsram[1811] sram->in sram[1811]->out sram[1811]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1811]->out) 0
.nodeset V(sram[1811]->outb) vsp
Xmux_1level_tapbuf_size2[170] grid[0][1]_pin[0][1][5] chanx[1][1]_in[77] chany[0][1]_out[21] sram[1812]->outb sram[1812]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[170], level=1, select_path_id=0. *****
*****1*****
Xsram[1812] sram->in sram[1812]->out sram[1812]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1812]->out) 0
.nodeset V(sram[1812]->outb) vsp
Xmux_1level_tapbuf_size2[171] grid[0][1]_pin[0][1][5] chanx[1][1]_in[75] chany[0][1]_out[23] sram[1813]->outb sram[1813]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[171], level=1, select_path_id=0. *****
*****1*****
Xsram[1813] sram->in sram[1813]->out sram[1813]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1813]->out) 0
.nodeset V(sram[1813]->outb) vsp
Xmux_1level_tapbuf_size2[172] grid[0][1]_pin[0][1][5] chanx[1][1]_in[73] chany[0][1]_out[25] sram[1814]->outb sram[1814]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[172], level=1, select_path_id=0. *****
*****1*****
Xsram[1814] sram->in sram[1814]->out sram[1814]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1814]->out) 0
.nodeset V(sram[1814]->outb) vsp
Xmux_1level_tapbuf_size2[173] grid[0][1]_pin[0][1][5] chanx[1][1]_in[71] chany[0][1]_out[27] sram[1815]->outb sram[1815]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[173], level=1, select_path_id=0. *****
*****1*****
Xsram[1815] sram->in sram[1815]->out sram[1815]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1815]->out) 0
.nodeset V(sram[1815]->outb) vsp
Xmux_1level_tapbuf_size2[174] grid[0][1]_pin[0][1][5] chanx[1][1]_in[69] chany[0][1]_out[29] sram[1816]->outb sram[1816]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[174], level=1, select_path_id=0. *****
*****1*****
Xsram[1816] sram->in sram[1816]->out sram[1816]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1816]->out) 0
.nodeset V(sram[1816]->outb) vsp
Xmux_1level_tapbuf_size2[175] grid[0][1]_pin[0][1][7] chanx[1][1]_in[67] chany[0][1]_out[31] sram[1817]->outb sram[1817]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[175], level=1, select_path_id=0. *****
*****1*****
Xsram[1817] sram->in sram[1817]->out sram[1817]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1817]->out) 0
.nodeset V(sram[1817]->outb) vsp
Xmux_1level_tapbuf_size2[176] grid[0][1]_pin[0][1][7] chanx[1][1]_in[65] chany[0][1]_out[33] sram[1818]->outb sram[1818]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[176], level=1, select_path_id=0. *****
*****1*****
Xsram[1818] sram->in sram[1818]->out sram[1818]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1818]->out) 0
.nodeset V(sram[1818]->outb) vsp
Xmux_1level_tapbuf_size2[177] grid[0][1]_pin[0][1][7] chanx[1][1]_in[63] chany[0][1]_out[35] sram[1819]->outb sram[1819]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[177], level=1, select_path_id=0. *****
*****1*****
Xsram[1819] sram->in sram[1819]->out sram[1819]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1819]->out) 0
.nodeset V(sram[1819]->outb) vsp
Xmux_1level_tapbuf_size2[178] grid[0][1]_pin[0][1][7] chanx[1][1]_in[61] chany[0][1]_out[37] sram[1820]->outb sram[1820]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[178], level=1, select_path_id=0. *****
*****1*****
Xsram[1820] sram->in sram[1820]->out sram[1820]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1820]->out) 0
.nodeset V(sram[1820]->outb) vsp
Xmux_1level_tapbuf_size2[179] grid[0][1]_pin[0][1][7] chanx[1][1]_in[59] chany[0][1]_out[39] sram[1821]->outb sram[1821]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[179], level=1, select_path_id=0. *****
*****1*****
Xsram[1821] sram->in sram[1821]->out sram[1821]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1821]->out) 0
.nodeset V(sram[1821]->outb) vsp
Xmux_1level_tapbuf_size2[180] grid[0][1]_pin[0][1][9] chanx[1][1]_in[57] chany[0][1]_out[41] sram[1822]->outb sram[1822]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[180], level=1, select_path_id=0. *****
*****1*****
Xsram[1822] sram->in sram[1822]->out sram[1822]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1822]->out) 0
.nodeset V(sram[1822]->outb) vsp
Xmux_1level_tapbuf_size2[181] grid[0][1]_pin[0][1][9] chanx[1][1]_in[55] chany[0][1]_out[43] sram[1823]->outb sram[1823]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[181], level=1, select_path_id=0. *****
*****1*****
Xsram[1823] sram->in sram[1823]->out sram[1823]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1823]->out) 0
.nodeset V(sram[1823]->outb) vsp
Xmux_1level_tapbuf_size2[182] grid[0][1]_pin[0][1][9] chanx[1][1]_in[53] chany[0][1]_out[45] sram[1824]->outb sram[1824]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[182], level=1, select_path_id=0. *****
*****1*****
Xsram[1824] sram->in sram[1824]->out sram[1824]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1824]->out) 0
.nodeset V(sram[1824]->outb) vsp
Xmux_1level_tapbuf_size2[183] grid[0][1]_pin[0][1][9] chanx[1][1]_in[51] chany[0][1]_out[47] sram[1825]->outb sram[1825]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[183], level=1, select_path_id=0. *****
*****1*****
Xsram[1825] sram->in sram[1825]->out sram[1825]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1825]->out) 0
.nodeset V(sram[1825]->outb) vsp
Xmux_1level_tapbuf_size2[184] grid[0][1]_pin[0][1][9] chanx[1][1]_in[49] chany[0][1]_out[49] sram[1826]->outb sram[1826]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[184], level=1, select_path_id=0. *****
*****1*****
Xsram[1826] sram->in sram[1826]->out sram[1826]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1826]->out) 0
.nodeset V(sram[1826]->outb) vsp
Xmux_1level_tapbuf_size2[185] grid[0][1]_pin[0][1][11] chanx[1][1]_in[47] chany[0][1]_out[51] sram[1827]->outb sram[1827]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[185], level=1, select_path_id=0. *****
*****1*****
Xsram[1827] sram->in sram[1827]->out sram[1827]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1827]->out) 0
.nodeset V(sram[1827]->outb) vsp
Xmux_1level_tapbuf_size2[186] grid[0][1]_pin[0][1][11] chanx[1][1]_in[45] chany[0][1]_out[53] sram[1828]->outb sram[1828]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[186], level=1, select_path_id=0. *****
*****1*****
Xsram[1828] sram->in sram[1828]->out sram[1828]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1828]->out) 0
.nodeset V(sram[1828]->outb) vsp
Xmux_1level_tapbuf_size2[187] grid[0][1]_pin[0][1][11] chanx[1][1]_in[43] chany[0][1]_out[55] sram[1829]->outb sram[1829]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[187], level=1, select_path_id=0. *****
*****1*****
Xsram[1829] sram->in sram[1829]->out sram[1829]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1829]->out) 0
.nodeset V(sram[1829]->outb) vsp
Xmux_1level_tapbuf_size2[188] grid[0][1]_pin[0][1][11] chanx[1][1]_in[41] chany[0][1]_out[57] sram[1830]->outb sram[1830]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[188], level=1, select_path_id=0. *****
*****1*****
Xsram[1830] sram->in sram[1830]->out sram[1830]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1830]->out) 0
.nodeset V(sram[1830]->outb) vsp
Xmux_1level_tapbuf_size2[189] grid[0][1]_pin[0][1][11] chanx[1][1]_in[39] chany[0][1]_out[59] sram[1831]->outb sram[1831]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[189], level=1, select_path_id=0. *****
*****1*****
Xsram[1831] sram->in sram[1831]->out sram[1831]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1831]->out) 0
.nodeset V(sram[1831]->outb) vsp
Xmux_1level_tapbuf_size2[190] grid[0][1]_pin[0][1][13] chanx[1][1]_in[37] chany[0][1]_out[61] sram[1832]->outb sram[1832]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[190], level=1, select_path_id=0. *****
*****1*****
Xsram[1832] sram->in sram[1832]->out sram[1832]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1832]->out) 0
.nodeset V(sram[1832]->outb) vsp
Xmux_1level_tapbuf_size2[191] grid[0][1]_pin[0][1][13] chanx[1][1]_in[35] chany[0][1]_out[63] sram[1833]->outb sram[1833]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[191], level=1, select_path_id=0. *****
*****1*****
Xsram[1833] sram->in sram[1833]->out sram[1833]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1833]->out) 0
.nodeset V(sram[1833]->outb) vsp
Xmux_1level_tapbuf_size2[192] grid[0][1]_pin[0][1][13] chanx[1][1]_in[33] chany[0][1]_out[65] sram[1834]->outb sram[1834]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[192], level=1, select_path_id=0. *****
*****1*****
Xsram[1834] sram->in sram[1834]->out sram[1834]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1834]->out) 0
.nodeset V(sram[1834]->outb) vsp
Xmux_1level_tapbuf_size2[193] grid[0][1]_pin[0][1][13] chanx[1][1]_in[31] chany[0][1]_out[67] sram[1835]->outb sram[1835]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[193], level=1, select_path_id=0. *****
*****1*****
Xsram[1835] sram->in sram[1835]->out sram[1835]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1835]->out) 0
.nodeset V(sram[1835]->outb) vsp
Xmux_1level_tapbuf_size2[194] grid[0][1]_pin[0][1][13] chanx[1][1]_in[29] chany[0][1]_out[69] sram[1836]->outb sram[1836]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[194], level=1, select_path_id=0. *****
*****1*****
Xsram[1836] sram->in sram[1836]->out sram[1836]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1836]->out) 0
.nodeset V(sram[1836]->outb) vsp
Xmux_1level_tapbuf_size2[195] grid[0][1]_pin[0][1][15] chanx[1][1]_in[27] chany[0][1]_out[71] sram[1837]->outb sram[1837]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[195], level=1, select_path_id=0. *****
*****1*****
Xsram[1837] sram->in sram[1837]->out sram[1837]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1837]->out) 0
.nodeset V(sram[1837]->outb) vsp
Xmux_1level_tapbuf_size2[196] grid[0][1]_pin[0][1][15] chanx[1][1]_in[25] chany[0][1]_out[73] sram[1838]->outb sram[1838]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[196], level=1, select_path_id=0. *****
*****1*****
Xsram[1838] sram->in sram[1838]->out sram[1838]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1838]->out) 0
.nodeset V(sram[1838]->outb) vsp
Xmux_1level_tapbuf_size2[197] grid[0][1]_pin[0][1][15] chanx[1][1]_in[23] chany[0][1]_out[75] sram[1839]->outb sram[1839]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[197], level=1, select_path_id=0. *****
*****1*****
Xsram[1839] sram->in sram[1839]->out sram[1839]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1839]->out) 0
.nodeset V(sram[1839]->outb) vsp
Xmux_1level_tapbuf_size2[198] grid[0][1]_pin[0][1][15] chanx[1][1]_in[21] chany[0][1]_out[77] sram[1840]->outb sram[1840]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[198], level=1, select_path_id=0. *****
*****1*****
Xsram[1840] sram->in sram[1840]->out sram[1840]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1840]->out) 0
.nodeset V(sram[1840]->outb) vsp
Xmux_1level_tapbuf_size2[199] grid[0][1]_pin[0][1][15] chanx[1][1]_in[19] chany[0][1]_out[79] sram[1841]->outb sram[1841]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[199], level=1, select_path_id=0. *****
*****1*****
Xsram[1841] sram->in sram[1841]->out sram[1841]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1841]->out) 0
.nodeset V(sram[1841]->outb) vsp
Xmux_1level_tapbuf_size2[200] grid[1][1]_pin[0][3][43] chanx[1][1]_in[17] chany[0][1]_out[81] sram[1842]->outb sram[1842]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[200], level=1, select_path_id=0. *****
*****1*****
Xsram[1842] sram->in sram[1842]->out sram[1842]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1842]->out) 0
.nodeset V(sram[1842]->outb) vsp
Xmux_1level_tapbuf_size2[201] grid[1][1]_pin[0][3][43] chanx[1][1]_in[15] chany[0][1]_out[83] sram[1843]->outb sram[1843]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[201], level=1, select_path_id=0. *****
*****1*****
Xsram[1843] sram->in sram[1843]->out sram[1843]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1843]->out) 0
.nodeset V(sram[1843]->outb) vsp
Xmux_1level_tapbuf_size2[202] grid[1][1]_pin[0][3][43] chanx[1][1]_in[13] chany[0][1]_out[85] sram[1844]->outb sram[1844]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[202], level=1, select_path_id=0. *****
*****1*****
Xsram[1844] sram->in sram[1844]->out sram[1844]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1844]->out) 0
.nodeset V(sram[1844]->outb) vsp
Xmux_1level_tapbuf_size2[203] grid[1][1]_pin[0][3][43] chanx[1][1]_in[11] chany[0][1]_out[87] sram[1845]->outb sram[1845]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[203], level=1, select_path_id=0. *****
*****1*****
Xsram[1845] sram->in sram[1845]->out sram[1845]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1845]->out) 0
.nodeset V(sram[1845]->outb) vsp
Xmux_1level_tapbuf_size2[204] grid[1][1]_pin[0][3][43] chanx[1][1]_in[9] chany[0][1]_out[89] sram[1846]->outb sram[1846]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[204], level=1, select_path_id=0. *****
*****1*****
Xsram[1846] sram->in sram[1846]->out sram[1846]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1846]->out) 0
.nodeset V(sram[1846]->outb) vsp
Xmux_1level_tapbuf_size2[205] grid[1][1]_pin[0][3][47] chanx[1][1]_in[7] chany[0][1]_out[91] sram[1847]->outb sram[1847]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[205], level=1, select_path_id=0. *****
*****1*****
Xsram[1847] sram->in sram[1847]->out sram[1847]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1847]->out) 0
.nodeset V(sram[1847]->outb) vsp
Xmux_1level_tapbuf_size2[206] grid[1][1]_pin[0][3][47] chanx[1][1]_in[5] chany[0][1]_out[93] sram[1848]->outb sram[1848]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[206], level=1, select_path_id=0. *****
*****1*****
Xsram[1848] sram->in sram[1848]->out sram[1848]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1848]->out) 0
.nodeset V(sram[1848]->outb) vsp
Xmux_1level_tapbuf_size2[207] grid[1][1]_pin[0][3][47] chanx[1][1]_in[3] chany[0][1]_out[95] sram[1849]->outb sram[1849]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[207], level=1, select_path_id=0. *****
*****1*****
Xsram[1849] sram->in sram[1849]->out sram[1849]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1849]->out) 0
.nodeset V(sram[1849]->outb) vsp
Xmux_1level_tapbuf_size2[208] grid[1][1]_pin[0][3][47] chanx[1][1]_in[1] chany[0][1]_out[97] sram[1850]->outb sram[1850]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[208], level=1, select_path_id=0. *****
*****1*****
Xsram[1850] sram->in sram[1850]->out sram[1850]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1850]->out) 0
.nodeset V(sram[1850]->outb) vsp
Xmux_1level_tapbuf_size2[209] grid[1][1]_pin[0][3][47] chanx[1][1]_in[99] chany[0][1]_out[99] sram[1851]->outb sram[1851]->out svdd sgnd mux_1level_tapbuf_size2
***** SRAM bits for MUX[209], level=1, select_path_id=0. *****
*****1*****
Xsram[1851] sram->in sram[1851]->out sram[1851]->outb gvdd_sram_sbs sgnd  sram6T
.nodeset V(sram[1851]->out) 0
.nodeset V(sram[1851]->outb) vsp
***** left side Multiplexers *****
.eom
