* Sub Circuit
* IO pads
* When direction = 0, pad <= dout
* When direction = 1, pad => din
.subckt iopad zin dout din pad direction direction_inv svdd sgnd
Xbuf0 pad din_inter svdd sgnd buf size=2
Xbuf1 dout pad_inter svdd sgnd buf size=2
*Xinv0 direction direction_inv svdd sgnd inv size=1
Xcpt0 din_inter din direction direction_inv svdd sgnd cpt
Xcpt1 pad_inter pad direction_inv direction svdd sgnd cpt
.eom iopad
