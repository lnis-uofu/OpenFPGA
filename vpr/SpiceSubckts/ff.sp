* Sub Circuits
*
* Static D Flip-flop
.subckt static_dff D Q clk set rst svdd sgnd
* Input inverter
Xinv_clk clk clk_b svdd sgnd inv
Xinv_set set set_b svdd sgnd inv
Xinv_rst rst rst_b svdd sgnd inv
Xinv0 D s1_n1 svdd sgnd inv 
Xcpt0 s1_n1 s1_n2 clk_b clk svdd sgnd cpt
Xset0 s1_n2 set_b svdd svdd vpr_pmos L=pl W='wp' 
Xrst0 s1_n2 rst sgnd sgnd vpr_nmos L=nl W='wn' 
Xinv1 s1_n2 s1_q svdd sgnd inv
Xinv2 s1_q s1_n3 svdd sgnd inv 
Xcpt1 s1_n3 s1_n2 clk_b clk svdd sgnd cpt 
* Stage 2
Xinv3 s1_q s2_n1 svdd sgnd inv 
Xcpt2 s2_n1 s2_n2 clk clk_b svdd sgnd cpt
Xset1 s2_n2 rst_b svdd svdd vpr_pmos L=pl W='wp' 
Xrst1 s2_n2 set sgnd sgnd vpr_nmos L=nl W='wn' 
Xinv4 s2_n2 Qb svdd sgnd inv
Xinv5 Qb s2_n3 svdd sgnd inv 
Xcpt3 s2_n3 s2_n2 clk clk_b svdd sgnd cpt 
Xinv_out Qb Q svdd sgnd inv
.eom static_dff
