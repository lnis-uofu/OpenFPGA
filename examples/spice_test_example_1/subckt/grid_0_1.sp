*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [0][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Grid[0][1] type_descriptor: io[0] *****
.subckt grid[0][1]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[0] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[0] sram[41]->outb sram[41]->out gvdd_iopad[0] sgnd iopad
***** SRAM bits for IOPAD[0] *****
*****1*****
Xsram[41] sram->in sram[41]->out sram[41]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[41]->out) 0
.nodeset V(sram[41]->outb) vsp
.eom
.subckt grid[0][1]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[14] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[15] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[1] *****
.subckt grid[0][1]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[1] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[1] sram[42]->outb sram[42]->out gvdd_iopad[1] sgnd iopad
***** SRAM bits for IOPAD[1] *****
*****1*****
Xsram[42] sram->in sram[42]->out sram[42]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[42]->out) 0
.nodeset V(sram[42]->outb) vsp
.eom
.subckt grid[0][1]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[16] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[17] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[2] *****
.subckt grid[0][1]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[2] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[2] sram[43]->outb sram[43]->out gvdd_iopad[2] sgnd iopad
***** SRAM bits for IOPAD[2] *****
*****1*****
Xsram[43] sram->in sram[43]->out sram[43]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[43]->out) 0
.nodeset V(sram[43]->outb) vsp
.eom
.subckt grid[0][1]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[18] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[19] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[3] *****
.subckt grid[0][1]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[3] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[3] sram[44]->outb sram[44]->out gvdd_iopad[3] sgnd iopad
***** SRAM bits for IOPAD[3] *****
*****1*****
Xsram[44] sram->in sram[44]->out sram[44]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[44]->out) 0
.nodeset V(sram[44]->outb) vsp
.eom
.subckt grid[0][1]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[20] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[21] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[4] *****
.subckt grid[0][1]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[4] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[4] sram[45]->outb sram[45]->out gvdd_iopad[4] sgnd iopad
***** SRAM bits for IOPAD[4] *****
*****1*****
Xsram[45] sram->in sram[45]->out sram[45]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[45]->out) 0
.nodeset V(sram[45]->outb) vsp
.eom
.subckt grid[0][1]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[22] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[23] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[5] *****
.subckt grid[0][1]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[5] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[5] sram[46]->outb sram[46]->out gvdd_iopad[5] sgnd iopad
***** SRAM bits for IOPAD[5] *****
*****1*****
Xsram[46] sram->in sram[46]->out sram[46]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[46]->out) 0
.nodeset V(sram[46]->outb) vsp
.eom
.subckt grid[0][1]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[24] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[25] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[6] *****
.subckt grid[0][1]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[6] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[6] sram[47]->outb sram[47]->out gvdd_iopad[6] sgnd iopad
***** SRAM bits for IOPAD[6] *****
*****1*****
Xsram[47] sram->in sram[47]->out sram[47]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[47]->out) 0
.nodeset V(sram[47]->outb) vsp
.eom
.subckt grid[0][1]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[26] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[27] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[7] *****
.subckt grid[0][1]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[7] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[7] sram[48]->outb sram[48]->out gvdd_iopad[7] sgnd iopad
***** SRAM bits for IOPAD[7] *****
*****1*****
Xsram[48] sram->in sram[48]->out sram[48]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[48]->out) 0
.nodeset V(sram[48]->outb) vsp
.eom
.subckt grid[0][1]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[28] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[29] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1], Capactity: 8 *****
***** Top Protocol *****
.subckt grid[0][1] 
+ right_height[0]_pin[0] 
+ right_height[0]_pin[1] 
+ right_height[0]_pin[2] 
+ right_height[0]_pin[3] 
+ right_height[0]_pin[4] 
+ right_height[0]_pin[5] 
+ right_height[0]_pin[6] 
+ right_height[0]_pin[7] 
+ right_height[0]_pin[8] 
+ right_height[0]_pin[9] 
+ right_height[0]_pin[10] 
+ right_height[0]_pin[11] 
+ right_height[0]_pin[12] 
+ right_height[0]_pin[13] 
+ right_height[0]_pin[14] 
+ right_height[0]_pin[15] 
+ svdd sgnd
Xgrid[0][1][0] 
+ right_height[0]_pin[0] 
+ right_height[0]_pin[1] 
+ svdd sgnd grid[0][1]_io[0]_mode[io_phy]
Xgrid[0][1][1] 
+ right_height[0]_pin[2] 
+ right_height[0]_pin[3] 
+ svdd sgnd grid[0][1]_io[1]_mode[io_phy]
Xgrid[0][1][2] 
+ right_height[0]_pin[4] 
+ right_height[0]_pin[5] 
+ svdd sgnd grid[0][1]_io[2]_mode[io_phy]
Xgrid[0][1][3] 
+ right_height[0]_pin[6] 
+ right_height[0]_pin[7] 
+ svdd sgnd grid[0][1]_io[3]_mode[io_phy]
Xgrid[0][1][4] 
+ right_height[0]_pin[8] 
+ right_height[0]_pin[9] 
+ svdd sgnd grid[0][1]_io[4]_mode[io_phy]
Xgrid[0][1][5] 
+ right_height[0]_pin[10] 
+ right_height[0]_pin[11] 
+ svdd sgnd grid[0][1]_io[5]_mode[io_phy]
Xgrid[0][1][6] 
+ right_height[0]_pin[12] 
+ right_height[0]_pin[13] 
+ svdd sgnd grid[0][1]_io[6]_mode[io_phy]
Xgrid[0][1][7] 
+ right_height[0]_pin[14] 
+ right_height[0]_pin[15] 
+ svdd sgnd grid[0][1]_io[7]_mode[io_phy]
.eom
