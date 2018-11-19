*****************************
*     FPGA SPICE Netlist    *
* Description: Inverter, Buffer, Trans. Gate *
*    Author: Xifan TANG     *
* Organization: EPFL/IC/LSI *
* Date: Thu Nov 15 14:26:08 2018
 *
*****************************
* Inverter
.subckt inv in out svdd sgnd size=1
Xn0_inv out in sgnd sgnd vpr_nmos L=nl W='size*wn'
Xp0_inv out in svdd svdd vpr_pmos L=pl W='size*beta*wp'
.eom inv

* Powergated Inverter
.subckt pg_inv en enb in out svdd sgnd size=1 pg_size=1
Xn0_inv out in sgnd_pg sgnd vpr_nmos L=nl W='size*wn'
Xp0_inv out in svdd_pg svdd vpr_pmos L=pl W='size*beta*wp'
Xn0_inv_pg sgnd_pg en sgnd sgnd vpr_nmos L=nl W='pg_size*wn'
Xp0_inv_pg svdd_pg enb svdd svdd vpr_pmos L=pl W='pg_size*beta*wp'
.eom inv

* Buffer
.subckt buf in out svdd sgnd size=2 base_size=1
Xinv0 in  mid svdd sgnd inv base_size='base_size'
Xinv1 mid out svdd sgnd inv size='size*base_size'
.eom buf

* Power-gated Buffer
.subckt pg_buf en enb in out svdd sgnd size=2 pg_size=2
Xinv0 en enb in  mid svdd sgnd pg_inv size=1 pg_size=1
Xinv1 en enb mid out svdd sgnd pg_inv size=size pg_size=size
.eom buf

* Transmission Gate (Complementary Pass Transistor)
.subckt cpt in out sel sel_inv svdd sgnd nmos_size=1 pmos_size=1
Xn0_cpt in sel out sgnd vpr_nmos L=nl W='nmos_size*wn'
Xp0_cpt in sel_inv out svdd vpr_pmos L=pl W='pmos_size*wp'
.eom cpt

.subckt tapbuf_level2_f4 in out svdd sgnd
Rinv_in in in_lvl0 0
Xinv_lvl0_no0 in_lvl0 in_lvl1 svdd sgnd inv
Xinv_lvl1_no0 in_lvl1 in_lvl2 svdd sgnd inv
Xinv_lvl1_no1 in_lvl1 in_lvl2 svdd sgnd inv
Xinv_lvl1_no2 in_lvl1 in_lvl2 svdd sgnd inv
Xinv_lvl1_no3 in_lvl1 in_lvl2 svdd sgnd inv
Rinv_out in_lvl2 out 0
.eom

.subckt tapbuf_level3_f4 in out svdd sgnd
Rinv_in in in_lvl0 0
Xinv_lvl0_no0 in_lvl0 in_lvl1 svdd sgnd inv
Xinv_lvl1_no0 in_lvl1 in_lvl2 svdd sgnd inv
Xinv_lvl1_no1 in_lvl1 in_lvl2 svdd sgnd inv
Xinv_lvl1_no2 in_lvl1 in_lvl2 svdd sgnd inv
Xinv_lvl1_no3 in_lvl1 in_lvl2 svdd sgnd inv
Xinv_lvl2_no0 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no1 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no2 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no3 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no4 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no5 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no6 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no7 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no8 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no9 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no10 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no11 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no12 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no13 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no14 in_lvl2 in_lvl3 svdd sgnd inv
Xinv_lvl2_no15 in_lvl2 in_lvl3 svdd sgnd inv
Rinv_out in_lvl3 out 0
.eom

