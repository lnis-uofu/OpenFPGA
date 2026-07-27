# Regenerate dual_port_ram_8x16_mif_yosys_out.eblif from dual_port_ram_8x16_mif.v
# Requires OpenFPGA-built Yosys and dpram8x16 preload techlib.
#
# Usage (from this directory):
#   ./gen_eblif.sh

set -euo pipefail
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OPENFPGA="$(cd "${SCRIPT_DIR}/../../.." && pwd)"
YOSYS="${OPENFPGA}/build/yosys/bin/yosys"
TECH="${OPENFPGA}/openfpga_flow/openfpga_yosys_techlib"

cd "${SCRIPT_DIR}"
"${YOSYS}" -q - <<EOF
read_verilog -nolatches dual_port_ram_8x16_mif.v
read_verilog -lib -specify ${TECH}/dpram8x16_preload_cell_sim.v
hierarchy -check -top dual_port_ram_128
proc
flatten
tribuf -logic
opt_expr
opt_clean
deminout
opt -nodffe -nosdff
opt_expr
opt_clean
check
opt -nodffe -nosdff
wreduce -keepdc
peepopt
pmuxtree
opt_clean
alumacc
share
opt -nodffe -nosdff
fsm
opt -fast
memory -nomap
opt_clean
memory_bram -rules ${TECH}/dpram8x16_preload_bram.txt
techmap -map ${TECH}/dpram8x16_preload_bram_map.v
opt -fast -mux_undef -undriven -fine -nodffe -nosdff
memory_map
opt -undriven -fine -nodffe -nosdff
dfflegalize -cell \$_DFF_P_ 0
techmap -map +/adff2dff.v
opt_expr -mux_undef
simplemap
opt_expr
opt_merge
opt_dff -nodffe -nosdff
opt_clean
opt -nodffe -nosdff
abc -lut 6
hierarchy -check
opt_clean -purge
write_blif -param dual_port_ram_8x16_mif_yosys_out.eblif
EOF

echo "Wrote ${SCRIPT_DIR}/dual_port_ram_8x16_mif_yosys_out.eblif"
