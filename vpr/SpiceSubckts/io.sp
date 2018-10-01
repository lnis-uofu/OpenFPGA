* Sub Circuit
* IO pads
* Inpad
.subckt inpad in out svdd sgnd
Xbuf0 in out svdd sgnd buf size=2
*Xinv0 in out svdd sgnd inv size=1
.eom inpad
* Outpad
.subckt outpad in out svdd sgnd
Xbuf0 in out svdd sgnd buf size=2
*Xinv0 in out svdd sgnd inv size=1
.eom outpad
