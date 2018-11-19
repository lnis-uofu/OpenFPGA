*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [2][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
***** Grid[2][1] type_descriptor: io[0] *****
.subckt grid[2][1]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[8] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[8] sram[1618]->outb sram[1618]->out gvdd_iopad[8] sgnd iopad
***** SRAM bits for IOPAD[8] *****
*****1*****
Xsram[1618] sram->in sram[1618]->out sram[1618]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1618]->out) 0
.nodeset V(sram[1618]->outb) vsp
.eom
.subckt grid[2][1]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[196] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[197] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[1] *****
.subckt grid[2][1]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[9] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[9] sram[1619]->outb sram[1619]->out gvdd_iopad[9] sgnd iopad
***** SRAM bits for IOPAD[9] *****
*****1*****
Xsram[1619] sram->in sram[1619]->out sram[1619]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1619]->out) 0
.nodeset V(sram[1619]->outb) vsp
.eom
.subckt grid[2][1]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[198] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[199] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[2] *****
.subckt grid[2][1]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[10] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[10] sram[1620]->outb sram[1620]->out gvdd_iopad[10] sgnd iopad
***** SRAM bits for IOPAD[10] *****
*****1*****
Xsram[1620] sram->in sram[1620]->out sram[1620]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1620]->out) 0
.nodeset V(sram[1620]->outb) vsp
.eom
.subckt grid[2][1]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[200] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[201] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[3] *****
.subckt grid[2][1]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[11] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[11] sram[1621]->outb sram[1621]->out gvdd_iopad[11] sgnd iopad
***** SRAM bits for IOPAD[11] *****
*****1*****
Xsram[1621] sram->in sram[1621]->out sram[1621]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1621]->out) 0
.nodeset V(sram[1621]->outb) vsp
.eom
.subckt grid[2][1]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[202] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[203] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[4] *****
.subckt grid[2][1]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[12] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[12] sram[1622]->outb sram[1622]->out gvdd_iopad[12] sgnd iopad
***** SRAM bits for IOPAD[12] *****
*****1*****
Xsram[1622] sram->in sram[1622]->out sram[1622]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1622]->out) 0
.nodeset V(sram[1622]->outb) vsp
.eom
.subckt grid[2][1]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[204] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[205] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[5] *****
.subckt grid[2][1]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[13] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[13] sram[1623]->outb sram[1623]->out gvdd_iopad[13] sgnd iopad
***** SRAM bits for IOPAD[13] *****
*****1*****
Xsram[1623] sram->in sram[1623]->out sram[1623]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1623]->out) 0
.nodeset V(sram[1623]->outb) vsp
.eom
.subckt grid[2][1]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[206] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[207] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[6] *****
.subckt grid[2][1]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[14] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[14] sram[1624]->outb sram[1624]->out gvdd_iopad[14] sgnd iopad
***** SRAM bits for IOPAD[14] *****
*****1*****
Xsram[1624] sram->in sram[1624]->out sram[1624]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1624]->out) 0
.nodeset V(sram[1624]->outb) vsp
.eom
.subckt grid[2][1]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[208] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[209] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[2][1] type_descriptor: io[7] *****
.subckt grid[2][1]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[15] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[15] sram[1625]->outb sram[1625]->out gvdd_iopad[15] sgnd iopad
***** SRAM bits for IOPAD[15] *****
*****1*****
Xsram[1625] sram->in sram[1625]->out sram[1625]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1625]->out) 0
.nodeset V(sram[1625]->outb) vsp
.eom
.subckt grid[2][1]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[2][1]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[210] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[211] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
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
