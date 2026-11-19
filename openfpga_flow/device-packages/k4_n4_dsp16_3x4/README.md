# k4_n4_dsp16_3x4

K4 N4 tileable fabric with a 16x16 fracturable multiplier (two 8x8 modes), 3x4 core

The multiplier `mult_16` runs as one 16x16 or as 8x8 slices; the mode is a bitstream setting.

Grid 3x4 (core), LUT size 4, channel width 58 for the reference design.
Made from the task `fpga_bitstream/extract_dsp_mode_bit`. The paths inside `arch/openfpga_arch.xml` are relative to this
directory. See `docs/spec_device_package.md`.
