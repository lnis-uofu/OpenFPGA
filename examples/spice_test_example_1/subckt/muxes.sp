*****************************
*     FPGA SPICE Netlist    *
* Description: MUXes used in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:04 2018
 *
*****************************
.subckt mux_2level_tapbuf_size4_basis in0 in1 out sel0 sel_inv0 sel1 sel_inv1 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='mux_2level_tapbuf_pgl_nmos_size' pmos_size='mux_2level_tapbuf_pgl_pmos_size'
Xcpt_1 in1 out sel1 sel_inv1 svdd sgnd cpt nmos_size='mux_2level_tapbuf_pgl_nmos_size' pmos_size='mux_2level_tapbuf_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=4, structure: multi-level *****
.subckt mux_2level_tapbuf_size4 in0 in1 in2 in3 out sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 svdd sgnd
Xmux_basis_no0 mux2_l2_in0 mux2_l2_in1 mux2_l1_in0 sram2 sram_inv2 sram3 sram_inv3 svdd sgnd mux_2level_tapbuf_size4_basis
Xmux_basis_no1 mux2_l2_in2 mux2_l2_in3 mux2_l1_in1 sram2 sram_inv2 sram3 sram_inv3 svdd sgnd mux_2level_tapbuf_size4_basis
Xmux_basis_no2 mux2_l1_in0 mux2_l1_in1 mux2_l0_in0 sram0 sram_inv0 sram1 sram_inv1 svdd sgnd mux_2level_tapbuf_size4_basis
Xinv0 in0 mux2_l2_in0 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv1 in1 mux2_l2_in1 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv2 in2 mux2_l2_in2 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv3 in3 mux2_l2_in3 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xbuf_out mux2_l0_in0 out svdd sgnd tapbuf_level3_f4
.eom
***** END CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=4 *****

.subckt lut4_size16_basis in0 in1 out sel0 sel_inv0 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='lut4_pgl_nmos_size' pmos_size='lut4_pgl_pmos_size'
Xcpt_1 in1 out sel_inv0 sel0 svdd sgnd cpt nmos_size='lut4_pgl_nmos_size' pmos_size='lut4_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name= lut4_MUX, size=16 *****
.subckt lut4_mux_size16 in0 in1 in2 in3 in4 in5 in6 in7 in8 in9 in10 in11 in12 in13 in14 in15 out sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 svdd sgnd
Xmux_basis_no0 mux2_l4_in0 mux2_l4_in1 mux2_l3_in0 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no1 mux2_l4_in2 mux2_l4_in3 mux2_l3_in1 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no2 mux2_l4_in4 mux2_l4_in5 mux2_l3_in2 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no3 mux2_l4_in6 mux2_l4_in7 mux2_l3_in3 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no4 mux2_l4_in8 mux2_l4_in9 mux2_l3_in4 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no5 mux2_l4_in10 mux2_l4_in11 mux2_l3_in5 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no6 mux2_l4_in12 mux2_l4_in13 mux2_l3_in6 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no7 mux2_l4_in14 mux2_l4_in15 mux2_l3_in7 sram0 sram_inv0 svdd sgnd lut4_size16_basis
Xmux_basis_no8 mux2_l3_in0 mux2_l3_in1 mux2_l2_in0 sram1 sram_inv1 svdd sgnd lut4_size16_basis
Xmux_basis_no9 mux2_l3_in2 mux2_l3_in3 mux2_l2_in1 sram1 sram_inv1 svdd sgnd lut4_size16_basis
Xmux_basis_no10 mux2_l3_in4 mux2_l3_in5 mux2_l2_in2 sram1 sram_inv1 svdd sgnd lut4_size16_basis
Xmux_basis_no11 mux2_l3_in6 mux2_l3_in7 mux2_l2_in3 sram1 sram_inv1 svdd sgnd lut4_size16_basis
Xmux_basis_no12 mux2_l2_in0 mux2_l2_in1 mux2_l1_in0 sram2 sram_inv2 svdd sgnd lut4_size16_basis
Xmux_basis_no13 mux2_l2_in2 mux2_l2_in3 mux2_l1_in1 sram2 sram_inv2 svdd sgnd lut4_size16_basis
Xmux_basis_no14 mux2_l1_in0 mux2_l1_in1 mux2_l0_in0 sram3 sram_inv3 svdd sgnd lut4_size16_basis
Xinv0 in0 mux2_l4_in0 svdd sgnd inv size='lut4_input_buf_size'
Xinv1 in1 mux2_l4_in1 svdd sgnd inv size='lut4_input_buf_size'
Xinv2 in2 mux2_l4_in2 svdd sgnd inv size='lut4_input_buf_size'
Xinv3 in3 mux2_l4_in3 svdd sgnd inv size='lut4_input_buf_size'
Xinv4 in4 mux2_l4_in4 svdd sgnd inv size='lut4_input_buf_size'
Xinv5 in5 mux2_l4_in5 svdd sgnd inv size='lut4_input_buf_size'
Xinv6 in6 mux2_l4_in6 svdd sgnd inv size='lut4_input_buf_size'
Xinv7 in7 mux2_l4_in7 svdd sgnd inv size='lut4_input_buf_size'
Xinv8 in8 mux2_l4_in8 svdd sgnd inv size='lut4_input_buf_size'
Xinv9 in9 mux2_l4_in9 svdd sgnd inv size='lut4_input_buf_size'
Xinv10 in10 mux2_l4_in10 svdd sgnd inv size='lut4_input_buf_size'
Xinv11 in11 mux2_l4_in11 svdd sgnd inv size='lut4_input_buf_size'
Xinv12 in12 mux2_l4_in12 svdd sgnd inv size='lut4_input_buf_size'
Xinv13 in13 mux2_l4_in13 svdd sgnd inv size='lut4_input_buf_size'
Xinv14 in14 mux2_l4_in14 svdd sgnd inv size='lut4_input_buf_size'
Xinv15 in15 mux2_l4_in15 svdd sgnd inv size='lut4_input_buf_size'
Xinv_out mux2_l0_in0 out svdd sgnd inv size='lut4_output_buf_size'
.eom
***** END CMOS MUX info: spice_model_name=lut4, size=16 *****

.subckt mux_2level_size5_basis in0 in1 in2 out sel0 sel_inv0 sel1 sel_inv1 sel2 sel_inv2 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_1 in1 out sel1 sel_inv1 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_2 in2 out sel2 sel_inv2 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name=mux_2level, size=5, structure: multi-level *****
.subckt mux_2level_size5 in0 in1 in2 in3 in4 out sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 sram4 sram_inv4 sram5 sram_inv5 svdd sgnd
Xmux_basis_no0 mux2_l2_in0 mux2_l2_in1 mux2_l2_in2 mux2_l1_in0 sram3 sram_inv3 sram4 sram_inv4 sram5 sram_inv5 svdd sgnd mux_2level_size5_basis
Xmux_basis_no1 mux2_l1_in0 mux2_l1_in1 mux2_l1_in2 mux2_l0_in0 sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 svdd sgnd mux_2level_size5_basis
Xinv0 in0 mux2_l2_in0 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv1 in1 mux2_l2_in1 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv2 in2 mux2_l2_in2 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv3 in3 mux2_l1_in1 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv4 in4 mux2_l1_in2 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv_out mux2_l0_in0 out svdd sgnd inv size='mux_2level_output_buf_size'
.eom
***** END CMOS MUX info: spice_model_name=mux_2level, size=5 *****

.subckt mux_1level_tapbuf_size2_basis in0 in1 out sel0 sel_inv0 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='mux_1level_tapbuf_pgl_nmos_size' pmos_size='mux_1level_tapbuf_pgl_pmos_size'
Xcpt_1 in1 out sel_inv0 sel0 svdd sgnd cpt nmos_size='mux_1level_tapbuf_pgl_nmos_size' pmos_size='mux_1level_tapbuf_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=2, structure: one-level *****
.subckt mux_1level_tapbuf_size2 in0 in1 out sram0 sram_inv0 svdd sgnd
Xmux_basis_no0 mux2_l1_in0 mux2_l1_in1 mux2_l0_in0 sram0 sram_inv0 svdd sgnd mux_1level_tapbuf_size2_basis
Xinv0 in0 mux2_l1_in0 svdd sgnd inv size='mux_1level_tapbuf_input_buf_size'
Xinv1 in1 mux2_l1_in1 svdd sgnd inv size='mux_1level_tapbuf_input_buf_size'
Xbuf_out mux2_l0_in0 out svdd sgnd tapbuf_level3_f4
.eom
***** END CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=2 *****

.subckt mux_1level_tapbuf_size3_basis in0 in1 in2 out sel0 sel_inv0 sel1 sel_inv1 sel2 sel_inv2 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='mux_1level_tapbuf_pgl_nmos_size' pmos_size='mux_1level_tapbuf_pgl_pmos_size'
Xcpt_1 in1 out sel1 sel_inv1 svdd sgnd cpt nmos_size='mux_1level_tapbuf_pgl_nmos_size' pmos_size='mux_1level_tapbuf_pgl_pmos_size'
Xcpt_2 in2 out sel2 sel_inv2 svdd sgnd cpt nmos_size='mux_1level_tapbuf_pgl_nmos_size' pmos_size='mux_1level_tapbuf_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=3, structure: one-level *****
.subckt mux_1level_tapbuf_size3 in0 in1 in2 out sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 svdd sgnd
Xmux_basis_no0 mux2_l1_in0 mux2_l1_in1 mux2_l1_in2 mux2_l0_in0 sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 svdd sgnd mux_1level_tapbuf_size3_basis
Xinv0 in0 mux2_l1_in0 svdd sgnd inv size='mux_1level_tapbuf_input_buf_size'
Xinv1 in1 mux2_l1_in1 svdd sgnd inv size='mux_1level_tapbuf_input_buf_size'
Xinv2 in2 mux2_l1_in2 svdd sgnd inv size='mux_1level_tapbuf_input_buf_size'
Xbuf_out mux2_l0_in0 out svdd sgnd tapbuf_level3_f4
.eom
***** END CMOS MUX info: spice_model_name=mux_1level_tapbuf, size=3 *****

