*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [2][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Grid[2][1] type_descriptor: io[0] *****
.subckt grid[2][1]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[8] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[8] sram[49]->outb sram[49]->out gvdd_iopad[8] sgnd iopad
***** SRAM bits for IOPAD[8] *****
*****1*****
Xsram[49] sram->in sram[49]->out sram[49]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[49]->out) 0
.nodeset V(sram[49]->outb) vsp
.eom
.subckt grid[2][1]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[30] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[31] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[1] *****
.subckt grid[2][1]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[9] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[9] sram[50]->outb sram[50]->out gvdd_iopad[9] sgnd iopad
***** SRAM bits for IOPAD[9] *****
*****1*****
Xsram[50] sram->in sram[50]->out sram[50]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[50]->out) 0
.nodeset V(sram[50]->outb) vsp
.eom
.subckt grid[2][1]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[32] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[33] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[2] *****
.subckt grid[2][1]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[10] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[10] sram[51]->outb sram[51]->out gvdd_iopad[10] sgnd iopad
***** SRAM bits for IOPAD[10] *****
*****1*****
Xsram[51] sram->in sram[51]->out sram[51]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[51]->out) 0
.nodeset V(sram[51]->outb) vsp
.eom
.subckt grid[2][1]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[34] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[35] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[3] *****
.subckt grid[2][1]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[11] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[11] sram[52]->outb sram[52]->out gvdd_iopad[11] sgnd iopad
***** SRAM bits for IOPAD[11] *****
*****1*****
Xsram[52] sram->in sram[52]->out sram[52]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[52]->out) 0
.nodeset V(sram[52]->outb) vsp
.eom
.subckt grid[2][1]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[36] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[37] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[4] *****
.subckt grid[2][1]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[12] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[12] sram[53]->outb sram[53]->out gvdd_iopad[12] sgnd iopad
***** SRAM bits for IOPAD[12] *****
*****1*****
Xsram[53] sram->in sram[53]->out sram[53]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[53]->out) 0
.nodeset V(sram[53]->outb) vsp
.eom
.subckt grid[2][1]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[38] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[39] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[5] *****
.subckt grid[2][1]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[13] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[13] sram[54]->outb sram[54]->out gvdd_iopad[13] sgnd iopad
***** SRAM bits for IOPAD[13] *****
*****1*****
Xsram[54] sram->in sram[54]->out sram[54]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[54]->out) 0
.nodeset V(sram[54]->outb) vsp
.eom
.subckt grid[2][1]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[40] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[41] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[6] *****
.subckt grid[2][1]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[14] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[14] sram[55]->outb sram[55]->out gvdd_iopad[14] sgnd iopad
***** SRAM bits for IOPAD[14] *****
*****1*****
Xsram[55] sram->in sram[55]->out sram[55]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[55]->out) 0
.nodeset V(sram[55]->outb) vsp
.eom
.subckt grid[2][1]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[42] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[43] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[7] *****
***** Logical block mapped to this IO: clk *****
.subckt grid[2][1]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[15] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[15] sram[56]->outb sram[56]->out gvdd_iopad[15] sgnd iopad
***** SRAM bits for IOPAD[15] *****
*****1*****
Xsram[56] sram->in sram[56]->out sram[56]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[56]->out) 0
.nodeset V(sram[56]->outb) vsp
.eom
.subckt grid[2][1]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[44] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[45] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1], Capactity: 8 *****
***** Top Protocol *****
.subckt grid[2][1] 
+ left_height[0]_pin[0] 
+ left_height[0]_pin[1] 
+ left_height[0]_pin[2] 
+ left_height[0]_pin[3] 
+ left_height[0]_pin[4] 
+ left_height[0]_pin[5] 
+ left_height[0]_pin[6] 
+ left_height[0]_pin[7] 
+ left_height[0]_pin[8] 
+ left_height[0]_pin[9] 
+ left_height[0]_pin[10] 
+ left_height[0]_pin[11] 
+ left_height[0]_pin[12] 
+ left_height[0]_pin[13] 
+ left_height[0]_pin[14] 
+ left_height[0]_pin[15] 
+ svdd sgnd
Xgrid[2][1][0] 
+ left_height[0]_pin[0] 
+ left_height[0]_pin[1] 
+ svdd sgnd grid[2][1]_io[0]_mode[io_phy]
Xgrid[2][1][1] 
+ left_height[0]_pin[2] 
+ left_height[0]_pin[3] 
+ svdd sgnd grid[2][1]_io[1]_mode[io_phy]
Xgrid[2][1][2] 
+ left_height[0]_pin[4] 
+ left_height[0]_pin[5] 
+ svdd sgnd grid[2][1]_io[2]_mode[io_phy]
Xgrid[2][1][3] 
+ left_height[0]_pin[6] 
+ left_height[0]_pin[7] 
+ svdd sgnd grid[2][1]_io[3]_mode[io_phy]
Xgrid[2][1][4] 
+ left_height[0]_pin[8] 
+ left_height[0]_pin[9] 
+ svdd sgnd grid[2][1]_io[4]_mode[io_phy]
Xgrid[2][1][5] 
+ left_height[0]_pin[10] 
+ left_height[0]_pin[11] 
+ svdd sgnd grid[2][1]_io[5]_mode[io_phy]
Xgrid[2][1][6] 
+ left_height[0]_pin[12] 
+ left_height[0]_pin[13] 
+ svdd sgnd grid[2][1]_io[6]_mode[io_phy]
Xgrid[2][1][7] 
+ left_height[0]_pin[14] 
+ left_height[0]_pin[15] 
+ svdd sgnd grid[2][1]_io[7]_mode[io_phy]
.eom
