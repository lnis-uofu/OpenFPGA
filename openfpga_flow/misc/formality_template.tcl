# = = = = = = = = = = = = = = = = = = = = = =
#  Auto generated using OpenFPGA
# = = = = = = = = = = = = = = = = = = = = = =

# Benchmark Source Files
read_verilog -container r -libname WORK -05 { ${SOURCE_DESIGN_FILES} }
set_top r:${SOURCE_TOP_MODULE}

# Benchmark Implementation Files
read_verilog -container i -libname WORK -05 { ${IMPL_DESIGN_FILES} }
set_top i:${IMPL_TOP_DIR}

match
# Port Mapping
${PORT_MAP_LIST}

# Register Mapping
${REGISTER_MAP_LIST}

verify
