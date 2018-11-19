*****************************
*     FPGA SPICE Netlist    *
* Description: Standard and I/O NMOS and PMOS *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
* Standard NMOS
.subckt vpr_nmos drain gate source bulk L=nl W=wn
M1 drain gate source bulk nch L=L W=W
.eom vpr_nmos

* Standard PMOS
.subckt vpr_pmos drain gate source bulk L=pl W=wp
M1 drain gate source bulk pch L=L W=W
.eom vpr_pmos
* I/O NMOS
.subckt vpr_io_nmos drain gate source bulk L=io_nl W=io_wn
M1 drain gate source bulk nch_25 L=L W=W
.eom vpr_io_nmos
* I/O PMOS
.subckt vpr_io_pmos drain gate source bulk L=io_pl W=io_wp
M1 drain gate source bulk pch_25 L=L W=W
.eom vpr_io_pmos
