Hierarchy of SPICE Output Files
===============================

All the generated SPICE netlists are located in the <spice_dir> as you specify in the command-line options.
Under the <spice_dir>, FPGA-SPICE creates a number of folders:  include, subckt, lut_tb, dff_tb, grid_tb, pb_mux_tb, cb_mux_tb, sb_mux_tb, top_tb, results. Under the <spice_dir>, FPGA-SPICE also creates a shell script called run_hspice_sim.sh, which run all the simulations for all the testbenches.
The folders contain the sub-circuits and testbenches and their contents are shown as following.

.. csv-table:: Folder hierarchy of FPGA-SPICE
   :header: "Folder", "Content includes the header files which contain the parameters for stimulate and measurement, as defined in <tech_lib>"
   :widths: 10, 20
    	
   "subckt", "Contain all the auto-generated sub-circuits, such as inverters, buffers, transmission gates, multiplexers, LUTs and even logic blocks, connection boxes and switch blocks."
   "lut_tb", "Contain all the testbenches for LUTs. This folder is created only when option print_spice_lut_testbench is enabled."
   "dff_tb", "Contain all the testbenches for FFs. This folder is created only when option print_spice_dff_testbench is enabled."
   "grid_tb", "Contain all the testbenches for logic blocks (grid-level testbenches). This folder is created only when option print_spice_grid_testbench is enabled."
   "pb_mux_tb", "Contain the testbenches for the multiplexers inside logic blocks. This folder is created only when option print_spice_pbmux_test-bench is enabled."
   "cb_mux_tb", "Contain all the testbenches for the multiplexers inside connection boxes. This folder is created only when option print_spice_cbmux_testbench is enabled."
   "sb_mux_tb", "Contain all the testbenches for the multiplexers inside switch blocks. This folder is created only when option print_spice_sbmux_test-bench is enabled."
   "top_tb", "Contain the full-chip-level  testbench. This folder is created only when option print_spice_top_testbench is enabled."
   "results", "An empty folder when created. It stores all the simulation results by running the shell script run_hspice_sim.sh."

