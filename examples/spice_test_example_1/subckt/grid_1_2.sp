*****************************
*     FPGA SPICE Netlist    *
* Description: Phyiscal Logic Block  [1][2] in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Grid[1][2] type_descriptor: io[0] *****
.subckt grid[1][2]_io[0]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[24] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[24] sram[65]->outb sram[65]->out gvdd_iopad[24] sgnd iopad
***** SRAM bits for IOPAD[24] *****
*****1*****
Xsram[65] sram->in sram[65]->out sram[65]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[65]->out) 0
.nodeset V(sram[65]->outb) vsp
.eom
.subckt grid[1][2]_io[0]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[0]_mode[io_phy]_iopad[0]
Xdirect_interc[62] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[63] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[1] *****
***** Logical block mapped to this IO: out_Q0 *****
.subckt grid[1][2]_io[1]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[25] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[25] sram[66]->out sram[66]->outb gvdd_iopad[25] sgnd iopad
***** SRAM bits for IOPAD[25] *****
*****0*****
Xsram[66] sram->in sram[66]->out sram[66]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[66]->out) 0
.nodeset V(sram[66]->outb) vsp
.eom
.subckt grid[1][2]_io[1]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[1]_mode[io_phy]_iopad[0]
Xdirect_interc[64] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[65] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[2] *****
.subckt grid[1][2]_io[2]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[26] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[26] sram[67]->outb sram[67]->out gvdd_iopad[26] sgnd iopad
***** SRAM bits for IOPAD[26] *****
*****1*****
Xsram[67] sram->in sram[67]->out sram[67]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[67]->out) 0
.nodeset V(sram[67]->outb) vsp
.eom
.subckt grid[1][2]_io[2]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[2]_mode[io_phy]_iopad[0]
Xdirect_interc[66] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[67] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[3] *****
.subckt grid[1][2]_io[3]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[27] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[27] sram[68]->outb sram[68]->out gvdd_iopad[27] sgnd iopad
***** SRAM bits for IOPAD[27] *****
*****1*****
Xsram[68] sram->in sram[68]->out sram[68]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[68]->out) 0
.nodeset V(sram[68]->outb) vsp
.eom
.subckt grid[1][2]_io[3]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[3]_mode[io_phy]_iopad[0]
Xdirect_interc[68] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[69] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[4] *****
.subckt grid[1][2]_io[4]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[28] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[28] sram[69]->outb sram[69]->out gvdd_iopad[28] sgnd iopad
***** SRAM bits for IOPAD[28] *****
*****1*****
Xsram[69] sram->in sram[69]->out sram[69]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[69]->out) 0
.nodeset V(sram[69]->outb) vsp
.eom
.subckt grid[1][2]_io[4]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[4]_mode[io_phy]_iopad[0]
Xdirect_interc[70] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[71] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[5] *****
.subckt grid[1][2]_io[5]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[29] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[29] sram[70]->outb sram[70]->out gvdd_iopad[29] sgnd iopad
***** SRAM bits for IOPAD[29] *****
*****1*****
Xsram[70] sram->in sram[70]->out sram[70]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[70]->out) 0
.nodeset V(sram[70]->outb) vsp
.eom
.subckt grid[1][2]_io[5]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[5]_mode[io_phy]_iopad[0]
Xdirect_interc[72] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[73] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[6] *****
***** Logical block mapped to this IO: I0 *****
.subckt grid[1][2]_io[6]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[30] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[30] sram[71]->outb sram[71]->out gvdd_iopad[30] sgnd iopad
***** SRAM bits for IOPAD[30] *****
*****1*****
Xsram[71] sram->in sram[71]->out sram[71]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[71]->out) 0
.nodeset V(sram[71]->outb) vsp
.eom
.subckt grid[1][2]_io[6]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[6]_mode[io_phy]_iopad[0]
Xdirect_interc[74] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[75] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
.eom
***** END *****

***** Grid[1][2] type_descriptor: io[7] *****
.subckt grid[1][2]_io[7]_mode[io_phy]_iopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd
Xiopad[31] 
***** BEGIN Global ports of SPICE_MODEL(iopad) *****
+  zin[0] 
***** END Global ports of SPICE_MODEL(iopad) *****
+ iopad[0]->outpad[0] iopad[0]->inpad[0]  gfpga_pad_iopad[31] sram[72]->outb sram[72]->out gvdd_iopad[31] sgnd iopad
***** SRAM bits for IOPAD[31] *****
*****1*****
Xsram[72] sram->in sram[72]->out sram[72]->outb gvdd_sram_io sgnd  sram6T
.nodeset V(sram[72]->out) 0
.nodeset V(sram[72]->outb) vsp
.eom
.subckt grid[1][2]_io[7]_mode[io_phy] mode[io_phy]->outpad[0] mode[io_phy]->inpad[0] svdd sgnd
Xiopad[0] iopad[0]->outpad[0] iopad[0]->inpad[0] svdd sgnd grid[1][2]_io[7]_mode[io_phy]_iopad[0]
Xdirect_interc[76] iopad[0]->inpad[0] mode[io_phy]->inpad[0] gvdd_local_interc sgnd direct_interc
Xdirect_interc[77] mode[io_phy]->outpad[0] iopad[0]->outpad[0] gvdd_local_interc sgnd direct_interc
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
