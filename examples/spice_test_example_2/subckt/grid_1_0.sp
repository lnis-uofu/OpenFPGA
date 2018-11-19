*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [1][0] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
***** Grid[1][0] type_descriptor: io[0] *****
.subckt grid[1][0]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[16] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[16] sram[1626]->outb sram[1626]->out gvdd_iopad[16] sgnd iopad
***** SRAM bits for IOPAD[16] *****
*****1*****
Xsram[1626] sram->in sram[1626]->out sram[1626]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1626]->out) 0
.nodeset V(sram[1626]->outb) vsp
.eom
.subckt grid[1][0]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[212] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[213] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[1] *****
***** Logical block mapped to this IO: I0 *****
.subckt grid[1][0]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[17] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[17] sram[1627]->outb sram[1627]->out gvdd_iopad[17] sgnd iopad
***** SRAM bits for IOPAD[17] *****
*****1*****
Xsram[1627] sram->in sram[1627]->out sram[1627]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1627]->out) 0
.nodeset V(sram[1627]->outb) vsp
.eom
.subckt grid[1][0]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[214] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[215] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[2] *****
.subckt grid[1][0]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[18] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[18] sram[1628]->outb sram[1628]->out gvdd_iopad[18] sgnd iopad
***** SRAM bits for IOPAD[18] *****
*****1*****
Xsram[1628] sram->in sram[1628]->out sram[1628]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1628]->out) 0
.nodeset V(sram[1628]->outb) vsp
.eom
.subckt grid[1][0]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[216] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[217] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[3] *****
.subckt grid[1][0]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[19] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[19] sram[1629]->outb sram[1629]->out gvdd_iopad[19] sgnd iopad
***** SRAM bits for IOPAD[19] *****
*****1*****
Xsram[1629] sram->in sram[1629]->out sram[1629]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1629]->out) 0
.nodeset V(sram[1629]->outb) vsp
.eom
.subckt grid[1][0]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[218] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[219] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[4] *****
.subckt grid[1][0]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[20] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[20] sram[1630]->outb sram[1630]->out gvdd_iopad[20] sgnd iopad
***** SRAM bits for IOPAD[20] *****
*****1*****
Xsram[1630] sram->in sram[1630]->out sram[1630]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1630]->out) 0
.nodeset V(sram[1630]->outb) vsp
.eom
.subckt grid[1][0]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[220] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[221] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[5] *****
.subckt grid[1][0]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[21] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[21] sram[1631]->outb sram[1631]->out gvdd_iopad[21] sgnd iopad
***** SRAM bits for IOPAD[21] *****
*****1*****
Xsram[1631] sram->in sram[1631]->out sram[1631]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1631]->out) 0
.nodeset V(sram[1631]->outb) vsp
.eom
.subckt grid[1][0]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[222] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[223] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[6] *****
.subckt grid[1][0]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[22] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[22] sram[1632]->outb sram[1632]->out gvdd_iopad[22] sgnd iopad
***** SRAM bits for IOPAD[22] *****
*****1*****
Xsram[1632] sram->in sram[1632]->out sram[1632]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1632]->out) 0
.nodeset V(sram[1632]->outb) vsp
.eom
.subckt grid[1][0]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[224] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[225] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][0] type_descriptor: io[7] *****
.subckt grid[1][0]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[23] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[23] sram[1633]->outb sram[1633]->out gvdd_iopad[23] sgnd iopad
***** SRAM bits for IOPAD[23] *****
*****1*****
Xsram[1633] sram->in sram[1633]->out sram[1633]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[1633]->out) 0
.nodeset V(sram[1633]->outb) vsp
.eom
.subckt grid[1][0]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][0]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[226] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[227] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
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
