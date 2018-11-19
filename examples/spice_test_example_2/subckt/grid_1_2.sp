*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [1][2] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
***** Grid[1][2] type_descriptor: io[0] *****
.subckt grid[1][2]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[24] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[24] sram[1634]->outb sram[1634]->out gvdd_iopad[24] sgnd iopad
***** SRAM bits for IOPAD[24] *****
*****1*****
Xsram[1634] sram->in sram[1634]->out sram[1634]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1634]->out) 0
.nodeset V(sram[1634]->outb) vsp
.eom
.subckt grid[1][2]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[228] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[229] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[1] *****
.subckt grid[1][2]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[25] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[25] sram[1635]->outb sram[1635]->out gvdd_iopad[25] sgnd iopad
***** SRAM bits for IOPAD[25] *****
*****1*****
Xsram[1635] sram->in sram[1635]->out sram[1635]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1635]->out) 0
.nodeset V(sram[1635]->outb) vsp
.eom
.subckt grid[1][2]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[230] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[231] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[2] *****
.subckt grid[1][2]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[26] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[26] sram[1636]->outb sram[1636]->out gvdd_iopad[26] sgnd iopad
***** SRAM bits for IOPAD[26] *****
*****1*****
Xsram[1636] sram->in sram[1636]->out sram[1636]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1636]->out) 0
.nodeset V(sram[1636]->outb) vsp
.eom
.subckt grid[1][2]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[232] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[233] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[3] *****
.subckt grid[1][2]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[27] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[27] sram[1637]->outb sram[1637]->out gvdd_iopad[27] sgnd iopad
***** SRAM bits for IOPAD[27] *****
*****1*****
Xsram[1637] sram->in sram[1637]->out sram[1637]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1637]->out) 0
.nodeset V(sram[1637]->outb) vsp
.eom
.subckt grid[1][2]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[234] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[235] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[4] *****
.subckt grid[1][2]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[28] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[28] sram[1638]->outb sram[1638]->out gvdd_iopad[28] sgnd iopad
***** SRAM bits for IOPAD[28] *****
*****1*****
Xsram[1638] sram->in sram[1638]->out sram[1638]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1638]->out) 0
.nodeset V(sram[1638]->outb) vsp
.eom
.subckt grid[1][2]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[236] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[237] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[5] *****
.subckt grid[1][2]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[29] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[29] sram[1639]->outb sram[1639]->out gvdd_iopad[29] sgnd iopad
***** SRAM bits for IOPAD[29] *****
*****1*****
Xsram[1639] sram->in sram[1639]->out sram[1639]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1639]->out) 0
.nodeset V(sram[1639]->outb) vsp
.eom
.subckt grid[1][2]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[238] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[239] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[6] *****
***** Logical block mapped to this IO: out_Q0 *****
.subckt grid[1][2]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[30] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[30] sram[1640]->out sram[1640]->outb gvdd_iopad[30] sgnd iopad
***** SRAM bits for IOPAD[30] *****
*****0*****
Xsram[1640] sram->in sram[1640]->out sram[1640]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1640]->out) 0
.nodeset V(sram[1640]->outb) vsp
.eom
.subckt grid[1][2]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[240] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[241] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[7] *****
.subckt grid[1][2]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[31] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[31] sram[1641]->outb sram[1641]->out gvdd_iopad[31] sgnd iopad
***** SRAM bits for IOPAD[31] *****
*****1*****
Xsram[1641] sram->in sram[1641]->out sram[1641]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1641]->out) 0
.nodeset V(sram[1641]->outb) vsp
.eom
.subckt grid[1][2]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[242] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[243] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2], Capactity: 8 *****
***** Top Protocol *****
.subckt grid[1][2] 
+ bottom_height[0]_pin[0] 
+ bottom_height[0]_pin[1] 
+ bottom_height[0]_pin[2] 
+ bottom_height[0]_pin[3] 
+ bottom_height[0]_pin[4] 
+ bottom_height[0]_pin[5] 
+ bottom_height[0]_pin[6] 
+ bottom_height[0]_pin[7] 
+ bottom_height[0]_pin[8] 
+ bottom_height[0]_pin[9] 
+ bottom_height[0]_pin[10] 
+ bottom_height[0]_pin[11] 
+ bottom_height[0]_pin[12] 
+ bottom_height[0]_pin[13] 
+ bottom_height[0]_pin[14] 
+ bottom_height[0]_pin[15] 
+ svdd sgnd
Xgrid[1][2][0] 
+ bottom_height[0]_pin[0] 
+ bottom_height[0]_pin[1] 
+ svdd sgnd grid[1][2]_io[0]_mode[io_phy]
Xgrid[1][2][1] 
+ bottom_height[0]_pin[2] 
+ bottom_height[0]_pin[3] 
+ svdd sgnd grid[1][2]_io[1]_mode[io_phy]
Xgrid[1][2][2] 
+ bottom_height[0]_pin[4] 
+ bottom_height[0]_pin[5] 
+ svdd sgnd grid[1][2]_io[2]_mode[io_phy]
Xgrid[1][2][3] 
+ bottom_height[0]_pin[6] 
+ bottom_height[0]_pin[7] 
+ svdd sgnd grid[1][2]_io[3]_mode[io_phy]
Xgrid[1][2][4] 
+ bottom_height[0]_pin[8] 
+ bottom_height[0]_pin[9] 
+ svdd sgnd grid[1][2]_io[4]_mode[io_phy]
Xgrid[1][2][5] 
+ bottom_height[0]_pin[10] 
+ bottom_height[0]_pin[11] 
+ svdd sgnd grid[1][2]_io[5]_mode[io_phy]
Xgrid[1][2][6] 
+ bottom_height[0]_pin[12] 
+ bottom_height[0]_pin[13] 
+ svdd sgnd grid[1][2]_io[6]_mode[io_phy]
Xgrid[1][2][7] 
+ bottom_height[0]_pin[14] 
+ bottom_height[0]_pin[15] 
+ svdd sgnd grid[1][2]_io[7]_mode[io_phy]
.eom
