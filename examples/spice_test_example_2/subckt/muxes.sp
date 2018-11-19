*****************************
*     FPGA SPICE Netlist    *
* Description: MUXes used in FPGA *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
.subckt mux_2level_tapbuf_size16_basis in0 in1 in2 in3 out sel0 sel_inv0 sel1 sel_inv1 sel2 sel_inv2 sel3 sel_inv3 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='mux_2level_tapbuf_pgl_nmos_size' pmos_size='mux_2level_tapbuf_pgl_pmos_size'
Xcpt_1 in1 out sel1 sel_inv1 svdd sgnd cpt nmos_size='mux_2level_tapbuf_pgl_nmos_size' pmos_size='mux_2level_tapbuf_pgl_pmos_size'
Xcpt_2 in2 out sel2 sel_inv2 svdd sgnd cpt nmos_size='mux_2level_tapbuf_pgl_nmos_size' pmos_size='mux_2level_tapbuf_pgl_pmos_size'
Xcpt_3 in3 out sel3 sel_inv3 svdd sgnd cpt nmos_size='mux_2level_tapbuf_pgl_nmos_size' pmos_size='mux_2level_tapbuf_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=16, structure: multi-level *****
.subckt mux_2level_tapbuf_size16 in0 in1 in2 in3 in4 in5 in6 in7 in8 in9 in10 in11 in12 in13 in14 in15 out sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 sram4 sram_inv4 sram5 sram_inv5 sram6 sram_inv6 sram7 sram_inv7 svdd sgnd
Xmux_basis_no0 mux2_l2_in0 mux2_l2_in1 mux2_l2_in2 mux2_l2_in3 mux2_l1_in0 sram4 sram_inv4 sram5 sram_inv5 sram6 sram_inv6 sram7 sram_inv7 svdd sgnd mux_2level_tapbuf_size16_basis
Xmux_basis_no1 mux2_l2_in4 mux2_l2_in5 mux2_l2_in6 mux2_l2_in7 mux2_l1_in1 sram4 sram_inv4 sram5 sram_inv5 sram6 sram_inv6 sram7 sram_inv7 svdd sgnd mux_2level_tapbuf_size16_basis
Xmux_basis_no2 mux2_l2_in8 mux2_l2_in9 mux2_l2_in10 mux2_l2_in11 mux2_l1_in2 sram4 sram_inv4 sram5 sram_inv5 sram6 sram_inv6 sram7 sram_inv7 svdd sgnd mux_2level_tapbuf_size16_basis
Xmux_basis_no3 mux2_l2_in12 mux2_l2_in13 mux2_l2_in14 mux2_l2_in15 mux2_l1_in3 sram4 sram_inv4 sram5 sram_inv5 sram6 sram_inv6 sram7 sram_inv7 svdd sgnd mux_2level_tapbuf_size16_basis
Xmux_basis_no4 mux2_l1_in0 mux2_l1_in1 mux2_l1_in2 mux2_l1_in3 mux2_l0_in0 sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 svdd sgnd mux_2level_tapbuf_size16_basis
Xinv0 in0 mux2_l2_in0 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv1 in1 mux2_l2_in1 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv2 in2 mux2_l2_in2 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv3 in3 mux2_l2_in3 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv4 in4 mux2_l2_in4 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv5 in5 mux2_l2_in5 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv6 in6 mux2_l2_in6 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv7 in7 mux2_l2_in7 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv8 in8 mux2_l2_in8 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv9 in9 mux2_l2_in9 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv10 in10 mux2_l2_in10 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv11 in11 mux2_l2_in11 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv12 in12 mux2_l2_in12 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv13 in13 mux2_l2_in13 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv14 in14 mux2_l2_in14 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xinv15 in15 mux2_l2_in15 svdd sgnd inv size='mux_2level_tapbuf_input_buf_size'
Xbuf_out mux2_l0_in0 out svdd sgnd tapbuf_level3_f4
.eom
***** END CMOS MUX info: spice_model_name=mux_2level_tapbuf, size=16 *****

.subckt lut6_size64_basis in0 in1 out sel0 sel_inv0 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='lut6_pgl_nmos_size' pmos_size='lut6_pgl_pmos_size'
Xcpt_1 in1 out sel_inv0 sel0 svdd sgnd cpt nmos_size='lut6_pgl_nmos_size' pmos_size='lut6_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name= lut6_MUX, size=64 *****
.subckt lut6_mux_size64 in0 in1 in2 in3 in4 in5 in6 in7 in8 in9 in10 in11 in12 in13 in14 in15 in16 in17 in18 in19 in20 in21 in22 in23 in24 in25 in26 in27 in28 in29 in30 in31 in32 in33 in34 in35 in36 in37 in38 in39 in40 in41 in42 in43 in44 in45 in46 in47 in48 in49 in50 in51 in52 in53 in54 in55 in56 in57 in58 in59 in60 in61 in62 in63 out sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 sram4 sram_inv4 sram5 sram_inv5 svdd sgnd
Xmux_basis_no0 mux2_l6_in0 mux2_l6_in1 mux2_l5_in0 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no1 mux2_l6_in2 mux2_l6_in3 mux2_l5_in1 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no2 mux2_l6_in4 mux2_l6_in5 mux2_l5_in2 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no3 mux2_l6_in6 mux2_l6_in7 mux2_l5_in3 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no4 mux2_l6_in8 mux2_l6_in9 mux2_l5_in4 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no5 mux2_l6_in10 mux2_l6_in11 mux2_l5_in5 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no6 mux2_l6_in12 mux2_l6_in13 mux2_l5_in6 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no7 mux2_l6_in14 mux2_l6_in15 mux2_l5_in7 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no8 mux2_l6_in16 mux2_l6_in17 mux2_l5_in8 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no9 mux2_l6_in18 mux2_l6_in19 mux2_l5_in9 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no10 mux2_l6_in20 mux2_l6_in21 mux2_l5_in10 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no11 mux2_l6_in22 mux2_l6_in23 mux2_l5_in11 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no12 mux2_l6_in24 mux2_l6_in25 mux2_l5_in12 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no13 mux2_l6_in26 mux2_l6_in27 mux2_l5_in13 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no14 mux2_l6_in28 mux2_l6_in29 mux2_l5_in14 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no15 mux2_l6_in30 mux2_l6_in31 mux2_l5_in15 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no16 mux2_l6_in32 mux2_l6_in33 mux2_l5_in16 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no17 mux2_l6_in34 mux2_l6_in35 mux2_l5_in17 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no18 mux2_l6_in36 mux2_l6_in37 mux2_l5_in18 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no19 mux2_l6_in38 mux2_l6_in39 mux2_l5_in19 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no20 mux2_l6_in40 mux2_l6_in41 mux2_l5_in20 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no21 mux2_l6_in42 mux2_l6_in43 mux2_l5_in21 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no22 mux2_l6_in44 mux2_l6_in45 mux2_l5_in22 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no23 mux2_l6_in46 mux2_l6_in47 mux2_l5_in23 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no24 mux2_l6_in48 mux2_l6_in49 mux2_l5_in24 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no25 mux2_l6_in50 mux2_l6_in51 mux2_l5_in25 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no26 mux2_l6_in52 mux2_l6_in53 mux2_l5_in26 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no27 mux2_l6_in54 mux2_l6_in55 mux2_l5_in27 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no28 mux2_l6_in56 mux2_l6_in57 mux2_l5_in28 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no29 mux2_l6_in58 mux2_l6_in59 mux2_l5_in29 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no30 mux2_l6_in60 mux2_l6_in61 mux2_l5_in30 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no31 mux2_l6_in62 mux2_l6_in63 mux2_l5_in31 sram0 sram_inv0 svdd sgnd lut6_size64_basis
Xmux_basis_no32 mux2_l5_in0 mux2_l5_in1 mux2_l4_in0 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no33 mux2_l5_in2 mux2_l5_in3 mux2_l4_in1 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no34 mux2_l5_in4 mux2_l5_in5 mux2_l4_in2 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no35 mux2_l5_in6 mux2_l5_in7 mux2_l4_in3 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no36 mux2_l5_in8 mux2_l5_in9 mux2_l4_in4 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no37 mux2_l5_in10 mux2_l5_in11 mux2_l4_in5 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no38 mux2_l5_in12 mux2_l5_in13 mux2_l4_in6 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no39 mux2_l5_in14 mux2_l5_in15 mux2_l4_in7 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no40 mux2_l5_in16 mux2_l5_in17 mux2_l4_in8 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no41 mux2_l5_in18 mux2_l5_in19 mux2_l4_in9 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no42 mux2_l5_in20 mux2_l5_in21 mux2_l4_in10 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no43 mux2_l5_in22 mux2_l5_in23 mux2_l4_in11 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no44 mux2_l5_in24 mux2_l5_in25 mux2_l4_in12 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no45 mux2_l5_in26 mux2_l5_in27 mux2_l4_in13 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no46 mux2_l5_in28 mux2_l5_in29 mux2_l4_in14 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no47 mux2_l5_in30 mux2_l5_in31 mux2_l4_in15 sram1 sram_inv1 svdd sgnd lut6_size64_basis
Xmux_basis_no48 mux2_l4_in0 mux2_l4_in1 mux2_l3_in0 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no49 mux2_l4_in2 mux2_l4_in3 mux2_l3_in1 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no50 mux2_l4_in4 mux2_l4_in5 mux2_l3_in2 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no51 mux2_l4_in6 mux2_l4_in7 mux2_l3_in3 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no52 mux2_l4_in8 mux2_l4_in9 mux2_l3_in4 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no53 mux2_l4_in10 mux2_l4_in11 mux2_l3_in5 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no54 mux2_l4_in12 mux2_l4_in13 mux2_l3_in6 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no55 mux2_l4_in14 mux2_l4_in15 mux2_l3_in7 sram2 sram_inv2 svdd sgnd lut6_size64_basis
Xmux_basis_no56 mux2_l3_in0 mux2_l3_in1 mux2_l2_in0 sram3 sram_inv3 svdd sgnd lut6_size64_basis
Xmux_basis_no57 mux2_l3_in2 mux2_l3_in3 mux2_l2_in1 sram3 sram_inv3 svdd sgnd lut6_size64_basis
Xmux_basis_no58 mux2_l3_in4 mux2_l3_in5 mux2_l2_in2 sram3 sram_inv3 svdd sgnd lut6_size64_basis
Xmux_basis_no59 mux2_l3_in6 mux2_l3_in7 mux2_l2_in3 sram3 sram_inv3 svdd sgnd lut6_size64_basis
Xmux_basis_no60 mux2_l2_in0 mux2_l2_in1 mux2_l1_in0 sram4 sram_inv4 svdd sgnd lut6_size64_basis
Xmux_basis_no61 mux2_l2_in2 mux2_l2_in3 mux2_l1_in1 sram4 sram_inv4 svdd sgnd lut6_size64_basis
Xmux_basis_no62 mux2_l1_in0 mux2_l1_in1 mux2_l0_in0 sram5 sram_inv5 svdd sgnd lut6_size64_basis
Xinv0 in0 mux2_l6_in0 svdd sgnd inv size='lut6_input_buf_size'
Xinv1 in1 mux2_l6_in1 svdd sgnd inv size='lut6_input_buf_size'
Xinv2 in2 mux2_l6_in2 svdd sgnd inv size='lut6_input_buf_size'
Xinv3 in3 mux2_l6_in3 svdd sgnd inv size='lut6_input_buf_size'
Xinv4 in4 mux2_l6_in4 svdd sgnd inv size='lut6_input_buf_size'
Xinv5 in5 mux2_l6_in5 svdd sgnd inv size='lut6_input_buf_size'
Xinv6 in6 mux2_l6_in6 svdd sgnd inv size='lut6_input_buf_size'
Xinv7 in7 mux2_l6_in7 svdd sgnd inv size='lut6_input_buf_size'
Xinv8 in8 mux2_l6_in8 svdd sgnd inv size='lut6_input_buf_size'
Xinv9 in9 mux2_l6_in9 svdd sgnd inv size='lut6_input_buf_size'
Xinv10 in10 mux2_l6_in10 svdd sgnd inv size='lut6_input_buf_size'
Xinv11 in11 mux2_l6_in11 svdd sgnd inv size='lut6_input_buf_size'
Xinv12 in12 mux2_l6_in12 svdd sgnd inv size='lut6_input_buf_size'
Xinv13 in13 mux2_l6_in13 svdd sgnd inv size='lut6_input_buf_size'
Xinv14 in14 mux2_l6_in14 svdd sgnd inv size='lut6_input_buf_size'
Xinv15 in15 mux2_l6_in15 svdd sgnd inv size='lut6_input_buf_size'
Xinv16 in16 mux2_l6_in16 svdd sgnd inv size='lut6_input_buf_size'
Xinv17 in17 mux2_l6_in17 svdd sgnd inv size='lut6_input_buf_size'
Xinv18 in18 mux2_l6_in18 svdd sgnd inv size='lut6_input_buf_size'
Xinv19 in19 mux2_l6_in19 svdd sgnd inv size='lut6_input_buf_size'
Xinv20 in20 mux2_l6_in20 svdd sgnd inv size='lut6_input_buf_size'
Xinv21 in21 mux2_l6_in21 svdd sgnd inv size='lut6_input_buf_size'
Xinv22 in22 mux2_l6_in22 svdd sgnd inv size='lut6_input_buf_size'
Xinv23 in23 mux2_l6_in23 svdd sgnd inv size='lut6_input_buf_size'
Xinv24 in24 mux2_l6_in24 svdd sgnd inv size='lut6_input_buf_size'
Xinv25 in25 mux2_l6_in25 svdd sgnd inv size='lut6_input_buf_size'
Xinv26 in26 mux2_l6_in26 svdd sgnd inv size='lut6_input_buf_size'
Xinv27 in27 mux2_l6_in27 svdd sgnd inv size='lut6_input_buf_size'
Xinv28 in28 mux2_l6_in28 svdd sgnd inv size='lut6_input_buf_size'
Xinv29 in29 mux2_l6_in29 svdd sgnd inv size='lut6_input_buf_size'
Xinv30 in30 mux2_l6_in30 svdd sgnd inv size='lut6_input_buf_size'
Xinv31 in31 mux2_l6_in31 svdd sgnd inv size='lut6_input_buf_size'
Xinv32 in32 mux2_l6_in32 svdd sgnd inv size='lut6_input_buf_size'
Xinv33 in33 mux2_l6_in33 svdd sgnd inv size='lut6_input_buf_size'
Xinv34 in34 mux2_l6_in34 svdd sgnd inv size='lut6_input_buf_size'
Xinv35 in35 mux2_l6_in35 svdd sgnd inv size='lut6_input_buf_size'
Xinv36 in36 mux2_l6_in36 svdd sgnd inv size='lut6_input_buf_size'
Xinv37 in37 mux2_l6_in37 svdd sgnd inv size='lut6_input_buf_size'
Xinv38 in38 mux2_l6_in38 svdd sgnd inv size='lut6_input_buf_size'
Xinv39 in39 mux2_l6_in39 svdd sgnd inv size='lut6_input_buf_size'
Xinv40 in40 mux2_l6_in40 svdd sgnd inv size='lut6_input_buf_size'
Xinv41 in41 mux2_l6_in41 svdd sgnd inv size='lut6_input_buf_size'
Xinv42 in42 mux2_l6_in42 svdd sgnd inv size='lut6_input_buf_size'
Xinv43 in43 mux2_l6_in43 svdd sgnd inv size='lut6_input_buf_size'
Xinv44 in44 mux2_l6_in44 svdd sgnd inv size='lut6_input_buf_size'
Xinv45 in45 mux2_l6_in45 svdd sgnd inv size='lut6_input_buf_size'
Xinv46 in46 mux2_l6_in46 svdd sgnd inv size='lut6_input_buf_size'
Xinv47 in47 mux2_l6_in47 svdd sgnd inv size='lut6_input_buf_size'
Xinv48 in48 mux2_l6_in48 svdd sgnd inv size='lut6_input_buf_size'
Xinv49 in49 mux2_l6_in49 svdd sgnd inv size='lut6_input_buf_size'
Xinv50 in50 mux2_l6_in50 svdd sgnd inv size='lut6_input_buf_size'
Xinv51 in51 mux2_l6_in51 svdd sgnd inv size='lut6_input_buf_size'
Xinv52 in52 mux2_l6_in52 svdd sgnd inv size='lut6_input_buf_size'
Xinv53 in53 mux2_l6_in53 svdd sgnd inv size='lut6_input_buf_size'
Xinv54 in54 mux2_l6_in54 svdd sgnd inv size='lut6_input_buf_size'
Xinv55 in55 mux2_l6_in55 svdd sgnd inv size='lut6_input_buf_size'
Xinv56 in56 mux2_l6_in56 svdd sgnd inv size='lut6_input_buf_size'
Xinv57 in57 mux2_l6_in57 svdd sgnd inv size='lut6_input_buf_size'
Xinv58 in58 mux2_l6_in58 svdd sgnd inv size='lut6_input_buf_size'
Xinv59 in59 mux2_l6_in59 svdd sgnd inv size='lut6_input_buf_size'
Xinv60 in60 mux2_l6_in60 svdd sgnd inv size='lut6_input_buf_size'
Xinv61 in61 mux2_l6_in61 svdd sgnd inv size='lut6_input_buf_size'
Xinv62 in62 mux2_l6_in62 svdd sgnd inv size='lut6_input_buf_size'
Xinv63 in63 mux2_l6_in63 svdd sgnd inv size='lut6_input_buf_size'
Xinv_out mux2_l0_in0 out svdd sgnd inv size='lut6_output_buf_size'
.eom
***** END CMOS MUX info: spice_model_name=lut6, size=64 *****

.subckt mux_2level_size50_basis in0 in1 in2 in3 in4 in5 in6 in7 out sel0 sel_inv0 sel1 sel_inv1 sel2 sel_inv2 sel3 sel_inv3 sel4 sel_inv4 sel5 sel_inv5 sel6 sel_inv6 sel7 sel_inv7 svdd sgnd
Xcpt_0 in0 out sel0 sel_inv0 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_1 in1 out sel1 sel_inv1 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_2 in2 out sel2 sel_inv2 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_3 in3 out sel3 sel_inv3 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_4 in4 out sel4 sel_inv4 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_5 in5 out sel5 sel_inv5 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_6 in6 out sel6 sel_inv6 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
Xcpt_7 in7 out sel7 sel_inv7 svdd sgnd cpt nmos_size='mux_2level_pgl_nmos_size' pmos_size='mux_2level_pgl_pmos_size'
.eom

***** CMOS MUX info: spice_model_name=mux_2level, size=50, structure: multi-level *****
.subckt mux_2level_size50 in0 in1 in2 in3 in4 in5 in6 in7 in8 in9 in10 in11 in12 in13 in14 in15 in16 in17 in18 in19 in20 in21 in22 in23 in24 in25 in26 in27 in28 in29 in30 in31 in32 in33 in34 in35 in36 in37 in38 in39 in40 in41 in42 in43 in44 in45 in46 in47 in48 in49 out sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 sram4 sram_inv4 sram5 sram_inv5 sram6 sram_inv6 sram7 sram_inv7 sram8 sram_inv8 sram9 sram_inv9 sram10 sram_inv10 sram11 sram_inv11 sram12 sram_inv12 sram13 sram_inv13 sram14 sram_inv14 sram15 sram_inv15 svdd sgnd
Xmux_basis_no0 mux2_l2_in0 mux2_l2_in1 mux2_l2_in2 mux2_l2_in3 mux2_l2_in4 mux2_l2_in5 mux2_l2_in6 mux2_l2_in7 mux2_l1_in0 sram8 sram_inv8 sram9 sram_inv9 sram10 sram_inv10 sram11 sram_inv11 sram12 sram_inv12 sram13 sram_inv13 sram14 sram_inv14 sram15 sram_inv15 svdd sgnd mux_2level_size50_basis
Xmux_basis_no1 mux2_l2_in8 mux2_l2_in9 mux2_l2_in10 mux2_l2_in11 mux2_l2_in12 mux2_l2_in13 mux2_l2_in14 mux2_l2_in15 mux2_l1_in1 sram8 sram_inv8 sram9 sram_inv9 sram10 sram_inv10 sram11 sram_inv11 sram12 sram_inv12 sram13 sram_inv13 sram14 sram_inv14 sram15 sram_inv15 svdd sgnd mux_2level_size50_basis
Xmux_basis_no2 mux2_l2_in16 mux2_l2_in17 mux2_l2_in18 mux2_l2_in19 mux2_l2_in20 mux2_l2_in21 mux2_l2_in22 mux2_l2_in23 mux2_l1_in2 sram8 sram_inv8 sram9 sram_inv9 sram10 sram_inv10 sram11 sram_inv11 sram12 sram_inv12 sram13 sram_inv13 sram14 sram_inv14 sram15 sram_inv15 svdd sgnd mux_2level_size50_basis
Xmux_basis_no3 mux2_l2_in24 mux2_l2_in25 mux2_l2_in26 mux2_l2_in27 mux2_l2_in28 mux2_l2_in29 mux2_l2_in30 mux2_l2_in31 mux2_l1_in3 sram8 sram_inv8 sram9 sram_inv9 sram10 sram_inv10 sram11 sram_inv11 sram12 sram_inv12 sram13 sram_inv13 sram14 sram_inv14 sram15 sram_inv15 svdd sgnd mux_2level_size50_basis
Xmux_basis_no4 mux2_l2_in32 mux2_l2_in33 mux2_l2_in34 mux2_l2_in35 mux2_l2_in36 mux2_l2_in37 mux2_l2_in38 mux2_l2_in39 mux2_l1_in4 sram8 sram_inv8 sram9 sram_inv9 sram10 sram_inv10 sram11 sram_inv11 sram12 sram_inv12 sram13 sram_inv13 sram14 sram_inv14 sram15 sram_inv15 svdd sgnd mux_2level_size50_basis
Xmux_basis_no5 mux2_l2_in40 mux2_l2_in41 mux2_l2_in42 mux2_l2_in43 mux2_l2_in44 mux2_l2_in45 mux2_l2_in46 mux2_l2_in47 mux2_l1_in5 sram8 sram_inv8 sram9 sram_inv9 sram10 sram_inv10 sram11 sram_inv11 sram12 sram_inv12 sram13 sram_inv13 sram14 sram_inv14 sram15 sram_inv15 svdd sgnd mux_2level_size50_basis
Xmux_basis_no6 mux2_l1_in0 mux2_l1_in1 mux2_l1_in2 mux2_l1_in3 mux2_l1_in4 mux2_l1_in5 mux2_l1_in6 mux2_l1_in7 mux2_l0_in0 sram0 sram_inv0 sram1 sram_inv1 sram2 sram_inv2 sram3 sram_inv3 sram4 sram_inv4 sram5 sram_inv5 sram6 sram_inv6 sram7 sram_inv7 svdd sgnd mux_2level_size50_basis
Xinv0 in0 mux2_l2_in0 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv1 in1 mux2_l2_in1 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv2 in2 mux2_l2_in2 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv3 in3 mux2_l2_in3 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv4 in4 mux2_l2_in4 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv5 in5 mux2_l2_in5 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv6 in6 mux2_l2_in6 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv7 in7 mux2_l2_in7 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv8 in8 mux2_l2_in8 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv9 in9 mux2_l2_in9 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv10 in10 mux2_l2_in10 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv11 in11 mux2_l2_in11 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv12 in12 mux2_l2_in12 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv13 in13 mux2_l2_in13 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv14 in14 mux2_l2_in14 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv15 in15 mux2_l2_in15 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv16 in16 mux2_l2_in16 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv17 in17 mux2_l2_in17 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv18 in18 mux2_l2_in18 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv19 in19 mux2_l2_in19 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv20 in20 mux2_l2_in20 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv21 in21 mux2_l2_in21 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv22 in22 mux2_l2_in22 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv23 in23 mux2_l2_in23 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv24 in24 mux2_l2_in24 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv25 in25 mux2_l2_in25 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv26 in26 mux2_l2_in26 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv27 in27 mux2_l2_in27 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv28 in28 mux2_l2_in28 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv29 in29 mux2_l2_in29 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv30 in30 mux2_l2_in30 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv31 in31 mux2_l2_in31 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv32 in32 mux2_l2_in32 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv33 in33 mux2_l2_in33 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv34 in34 mux2_l2_in34 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv35 in35 mux2_l2_in35 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv36 in36 mux2_l2_in36 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv37 in37 mux2_l2_in37 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv38 in38 mux2_l2_in38 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv39 in39 mux2_l2_in39 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv40 in40 mux2_l2_in40 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv41 in41 mux2_l2_in41 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv42 in42 mux2_l2_in42 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv43 in43 mux2_l2_in43 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv44 in44 mux2_l2_in44 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv45 in45 mux2_l2_in45 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv46 in46 mux2_l2_in46 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv47 in47 mux2_l2_in47 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv48 in48 mux2_l1_in6 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv49 in49 mux2_l1_in7 svdd sgnd inv size='mux_2level_input_buf_size'
Xinv_out mux2_l0_in0 out svdd sgnd inv size='mux_2level_output_buf_size'
.eom
***** END CMOS MUX info: spice_model_name=mux_2level, size=50 *****

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

