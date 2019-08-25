* Sub Circuit
* OR2 gate
.subckt or2 in0 in1 out svdd sgnd size=1
Xp0 ntwk_n0 in0 svdd svdd vpr_pmos L=pl W='size*beta*wp'
Xp1 ntwk_n0 in1 ntwk_n1 svdd vpr_pmos L=pl W='size*beta*wp'
Xn0 ntwk_n1 in0 sgnd sgnd vpr_nmos L=nl W='wn*size'
Xn1 ntwk_n1 in1 sgnd sgnd vpr_nmos L=nl W='wn*size'
.eom

* AND2 gate
.subckt and2 in0 in1 out svdd sgnd size=1
Xp0 ntwk_n0 in0 svdd svdd vpr_pmos L=pl W='wp*size*beta'
Xp1 ntwk_n0 in1 svdd svdd vpr_pmos L=pl W='wp*size*beta'
Xn0 ntwk_n0 in0 ntwk_n1 sgnd vpr_nmos L=nl W='wn*size'
Xn1 ntwk_n1 in1 sgnd sgnd vpr_nmos L=nl W='wn*size'
.eom
