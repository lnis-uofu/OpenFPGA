* Sub Circuit
* SRAM
* Input to force write the stored bit
.subckt sram6T in out outb svdd sgnd
Xinv0 loop_out loop_outb svdd sgnd inv size=1
Xinv1 loop_outb loop_out svdd sgnd inv size=1
Xout_pt loop_out out svdd sgnd svdd sgnd cpt
Xoutb_pt loop_outb outb svdd sgnd svdd sgnd cpt
Vin in loop_out 0 
.eom sram6T
