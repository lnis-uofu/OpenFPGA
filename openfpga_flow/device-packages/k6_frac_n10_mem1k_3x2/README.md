# k6_frac_n10_mem1k_3x2

K6 N10 tileable fabric with adder chains and 1 kbit dual-port memory, 3x2 core

The memory is the 1 kbit dual-port block `dpram1k`.

Grid 3x2 (core), LUT size 6, channel width 50 for the reference design.
Made from the task `fpga_verilog/bram/dpram1k`. The paths inside `arch/openfpga_arch.xml` are relative to this
directory. See `docs/spec_device_package.md`.
