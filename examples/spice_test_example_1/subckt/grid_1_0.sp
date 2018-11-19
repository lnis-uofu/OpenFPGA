*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [1][0] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Grid[1][0] type_descriptor: io[0] *****
.subckt grid[1][0]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[16] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[16] sram[57]->outb sram[57]->out gvdd_iopad[16] sgnd iopad
***** SRAM bits for IOPAD[16] *****
*****1*****
Xsram[57] sram->in sram[57]->out sram[57]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[57]->out) 0
.nodeset V(sram[57]->outb) vsp
.eom
.subckt grid[1][0]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[46] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[47] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[1] *****
.subckt grid[1][0]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[17] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[17] sram[58]->outb sram[58]->out gvdd_iopad[17] sgnd iopad
***** SRAM bits for IOPAD[17] *****
*****1*****
Xsram[58] sram->in sram[58]->out sram[58]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[58]->out) 0
.nodeset V(sram[58]->outb) vsp
.eom
.subckt grid[1][0]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[48] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[49] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[2] *****
.subckt grid[1][0]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[18] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[18] sram[59]->outb sram[59]->out gvdd_iopad[18] sgnd iopad
***** SRAM bits for IOPAD[18] *****
*****1*****
Xsram[59] sram->in sram[59]->out sram[59]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[59]->out) 0
.nodeset V(sram[59]->outb) vsp
.eom
.subckt grid[1][0]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[50] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[51] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[3] *****
.subckt grid[1][0]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[19] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[19] sram[60]->outb sram[60]->out gvdd_iopad[19] sgnd iopad
***** SRAM bits for IOPAD[19] *****
*****1*****
Xsram[60] sram->in sram[60]->out sram[60]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[60]->out) 0
.nodeset V(sram[60]->outb) vsp
.eom
.subckt grid[1][0]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[52] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[53] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[4] *****
.subckt grid[1][0]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[20] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[20] sram[61]->outb sram[61]->out gvdd_iopad[20] sgnd iopad
***** SRAM bits for IOPAD[20] *****
*****1*****
Xsram[61] sram->in sram[61]->out sram[61]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[61]->out) 0
.nodeset V(sram[61]->outb) vsp
.eom
.subckt grid[1][0]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[54] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[55] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[5] *****
.subckt grid[1][0]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[21] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[21] sram[62]->outb sram[62]->out gvdd_iopad[21] sgnd iopad
***** SRAM bits for IOPAD[21] *****
*****1*****
Xsram[62] sram->in sram[62]->out sram[62]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[62]->out) 0
.nodeset V(sram[62]->outb) vsp
.eom
.subckt grid[1][0]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[56] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[57] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[6] *****
.subckt grid[1][0]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[22] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[22] sram[63]->outb sram[63]->out gvdd_iopad[22] sgnd iopad
***** SRAM bits for IOPAD[22] *****
*****1*****
Xsram[63] sram->in sram[63]->out sram[63]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[63]->out) 0
.nodeset V(sram[63]->outb) vsp
.eom
.subckt grid[1][0]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[58] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[59] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[7] *****
.subckt grid[1][0]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[23] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[23] sram[64]->outb sram[64]->out gvdd_iopad[23] sgnd iopad
***** SRAM bits for IOPAD[23] *****
*****1*****
Xsram[64] sram->in sram[64]->out sram[64]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[64]->out) 0
.nodeset V(sram[64]->outb) vsp
.eom
.subckt grid[1][0]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[60] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[61] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0], Capactity: 8 *****
***** Top Protocol *****
.subckt grid[1][0] 
+ top_height[0]_pin[0] 
+ top_height[0]_pin[1] 
+ top_height[0]_pin[2] 
+ top_height[0]_pin[3] 
+ top_height[0]_pin[4] 
+ top_height[0]_pin[5] 
+ top_height[0]_pin[6] 
+ top_height[0]_pin[7] 
+ top_height[0]_pin[8] 
+ top_height[0]_pin[9] 
+ top_height[0]_pin[10] 
+ top_height[0]_pin[11] 
+ top_height[0]_pin[12] 
+ top_height[0]_pin[13] 
+ top_height[0]_pin[14] 
+ top_height[0]_pin[15] 
+ svdd sgnd
Xgrid[1][0][0] 
+ top_height[0]_pin[0] 
+ top_height[0]_pin[1] 
+ svdd sgnd grid[1][0]_io[0]_mode[io_phy]
Xgrid[1][0][1] 
+ top_height[0]_pin[2] 
+ top_height[0]_pin[3] 
+ svdd sgnd grid[1][0]_io[1]_mode[io_phy]
Xgrid[1][0][2] 
+ top_height[0]_pin[4] 
+ top_height[0]_pin[5] 
+ svdd sgnd grid[1][0]_io[2]_mode[io_phy]
Xgrid[1][0][3] 
+ top_height[0]_pin[6] 
+ top_height[0]_pin[7] 
+ svdd sgnd grid[1][0]_io[3]_mode[io_phy]
Xgrid[1][0][4] 
+ top_height[0]_pin[8] 
+ top_height[0]_pin[9] 
+ svdd sgnd grid[1][0]_io[4]_mode[io_phy]
Xgrid[1][0][5] 
+ top_height[0]_pin[10] 
+ top_height[0]_pin[11] 
+ svdd sgnd grid[1][0]_io[5]_mode[io_phy]
Xgrid[1][0][6] 
+ top_height[0]_pin[12] 
+ top_height[0]_pin[13] 
+ svdd sgnd grid[1][0]_io[6]_mode[io_phy]
Xgrid[1][0][7] 
+ top_height[0]_pin[14] 
+ top_height[0]_pin[15] 
+ svdd sgnd grid[1][0]_io[7]_mode[io_phy]
.eom
