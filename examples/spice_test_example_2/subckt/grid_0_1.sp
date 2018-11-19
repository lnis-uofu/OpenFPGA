*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [0][1] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
***** Grid[0][1] type_descriptor: io[0] *****
.subckt grid[0][1]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[0] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[0] sram[1610]->outb sram[1610]->out gvdd_iopad[0] sgnd iopad
***** SRAM bits for IOPAD[0] *****
*****1*****
Xsram[1610] sram->in sram[1610]->out sram[1610]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1610]->out) 0
.nodeset V(sram[1610]->outb) vsp
.eom
.subckt grid[0][1]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[180] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[181] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[1] *****
***** Logical block mapped to this IO: clk *****
.subckt grid[0][1]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[1] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[1] sram[1611]->outb sram[1611]->out gvdd_iopad[1] sgnd iopad
***** SRAM bits for IOPAD[1] *****
*****1*****
Xsram[1611] sram->in sram[1611]->out sram[1611]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1611]->out) 0
.nodeset V(sram[1611]->outb) vsp
.eom
.subckt grid[0][1]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[182] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[183] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[2] *****
.subckt grid[0][1]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[2] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[2] sram[1612]->outb sram[1612]->out gvdd_iopad[2] sgnd iopad
***** SRAM bits for IOPAD[2] *****
*****1*****
Xsram[1612] sram->in sram[1612]->out sram[1612]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1612]->out) 0
.nodeset V(sram[1612]->outb) vsp
.eom
.subckt grid[0][1]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[184] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[185] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[3] *****
.subckt grid[0][1]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[3] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[3] sram[1613]->outb sram[1613]->out gvdd_iopad[3] sgnd iopad
***** SRAM bits for IOPAD[3] *****
*****1*****
Xsram[1613] sram->in sram[1613]->out sram[1613]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1613]->out) 0
.nodeset V(sram[1613]->outb) vsp
.eom
.subckt grid[0][1]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[186] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[187] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[4] *****
.subckt grid[0][1]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[4] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[4] sram[1614]->outb sram[1614]->out gvdd_iopad[4] sgnd iopad
***** SRAM bits for IOPAD[4] *****
*****1*****
Xsram[1614] sram->in sram[1614]->out sram[1614]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1614]->out) 0
.nodeset V(sram[1614]->outb) vsp
.eom
.subckt grid[0][1]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[188] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[189] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[5] *****
.subckt grid[0][1]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[5] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[5] sram[1615]->outb sram[1615]->out gvdd_iopad[5] sgnd iopad
***** SRAM bits for IOPAD[5] *****
*****1*****
Xsram[1615] sram->in sram[1615]->out sram[1615]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1615]->out) 0
.nodeset V(sram[1615]->outb) vsp
.eom
.subckt grid[0][1]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[190] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[191] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[6] *****
.subckt grid[0][1]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[6] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[6] sram[1616]->outb sram[1616]->out gvdd_iopad[6] sgnd iopad
***** SRAM bits for IOPAD[6] *****
*****1*****
Xsram[1616] sram->in sram[1616]->out sram[1616]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1616]->out) 0
.nodeset V(sram[1616]->outb) vsp
.eom
.subckt grid[0][1]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[192] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[193] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[0][1] type_descriptor: io[7] *****
.subckt grid[0][1]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[7] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[7] sram[1617]->outb sram[1617]->out gvdd_iopad[7] sgnd iopad
***** SRAM bits for IOPAD[7] *****
*****1*****
Xsram[1617] sram->in sram[1617]->out sram[1617]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1617]->out) 0
.nodeset V(sram[1617]->outb) vsp
.eom
.subckt grid[0][1]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[0][1]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[194] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[195] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
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
