* Sub Circuits
*
* Static D Flip-flop
.subckt static_dff set rst clk D Q svdd sgnd size=1
* Input inverter
Xinv_clk clk clk_b svdd sgnd inv size=size
Xinv_set set set_b svdd sgnd inv size=size
Xinv_rst rst rst_b svdd sgnd inv size=size
Xinv_d D s1_n1 svdd sgnd inv size=size
Xcpt0 s1_n1 s1_n2 clk_b clk svdd sgnd cpt nmos_size='size' pmos_size='size*beta'
Xset0 s1_n2 set_b svdd svdd vpr_pmos L=pl W='size*wp' 
Xrst0 s1_n2 rst sgnd sgnd vpr_nmos L=nl W='size*wn' 
Xinv1 s1_n2 s1_q svdd sgnd inv size=size
Xinv2 s1_q s1_n3 svdd sgnd inv size=size 
Xcpt1 s1_n3 s1_n2 clk clk_b svdd sgnd cpt nmos_size='size' pmos_size='size*beta' 
* Stage 2
R3 s1_q s2_n1 0 
Xcpt2 s2_n1 s2_n2 clk clk_b svdd sgnd cpt nmos_size='size' pmos_size='size*beta'
Xrst1 s2_n2 rst_b svdd svdd vpr_pmos L=pl W='size*wp' 
Xset1 s2_n2 set sgnd sgnd vpr_nmos L=nl W='size*wn' 
Xinv4 s2_n2 Qb svdd sgnd inv size=size 
Xinv5 Qb s2_n3 svdd sgnd inv size=size  
Xcpt3 s2_n3 s2_n2 clk_b clk svdd sgnd cpt nmos_size='size' pmos_size='size*beta' 
Xinv_out Qb Q svdd sgnd inv size=size 
.eom static_dff
