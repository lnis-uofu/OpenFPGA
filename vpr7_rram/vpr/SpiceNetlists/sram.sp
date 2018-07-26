* Sub Circuit
* SRAM
* Input to force write the stored bit
.subckt sram6T in out outb svdd sgnd size=1
Xinv0 loop_out loop_outb svdd sgnd inv size=size
Xinv1 loop_outb loop_out svdd sgnd inv size=size
Xout_pt loop_out out svdd sgnd svdd sgnd cpt nmos_size='size' pmos_size='size*beta'
Xoutb_pt loop_outb outb svdd sgnd svdd sgnd cpt 
Rin in loop_out 0 
.eom sram6T
