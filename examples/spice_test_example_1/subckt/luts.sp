*****************************
*     FPGA SPICE Netlist    *
* Description: LUTs *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
***** Auto-generated LUT info: spice_model_name = lut4, size = 4 *****
.subckt lut4 in0 in1 in2 in3 out sram0 sram1 sram2 sram3 sram4 sram5 sram6 sram7 sram8 sram9 sram10 sram11 sram12 sram13 sram14 sram15 svdd sgnd
Xinv0_in0_no0 in0 lut_mux_in0_inv svdd sgnd inv size='4'
Xbuf4_in0 in0 lut_mux_in0 svdd sgnd tapbuf_level2_f4

Xinv0_in1_no0 in1 lut_mux_in1_inv svdd sgnd inv size='4'
Xbuf4_in1 in1 lut_mux_in1 svdd sgnd tapbuf_level2_f4

Xinv0_in2_no0 in2 lut_mux_in2_inv svdd sgnd inv size='4'
Xbuf4_in2 in2 lut_mux_in2 svdd sgnd tapbuf_level2_f4

Xinv0_in3_no0 in3 lut_mux_in3_inv svdd sgnd inv size='4'
Xbuf4_in3 in3 lut_mux_in3 svdd sgnd tapbuf_level2_f4

Xlut_mux sram0 sram1 sram2 sram3 sram4 sram5 sram6 sram7 sram8 sram9 sram10 sram11 sram12 sram13 sram14 sram15 out lut_mux_in0 lut_mux_in0_inv lut_mux_in1 lut_mux_in1_inv lut_mux_in2 lut_mux_in2_inv lut_mux_in3 lut_mux_in3_inv svdd sgnd lut4_mux_size16
.eom
