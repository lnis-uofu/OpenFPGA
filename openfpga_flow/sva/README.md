To compile the following testbenches :
- You need to add both testbench & sva_checker to your project.
- You can enable/disable the checker by changing the "enable_assertions" parameter in the testbench
     bind fpga_top inv_checker #(.enable_assertions(1)) when you need to enable assertions
     bind fpga_top inv_checker #(.enable_assertions(0)) when you need to disable assertions
Disabling assertions for the current architecture saves about 3% simulation runtime.
- The checker is tuned for 4clb with 20FF each 2x2, bitstream lenght & number of flipflop is set by default for this specific architecture but it can be modified through BS_LGT & FF_N parameter inside the testbench.

Descriptions of features being checked are written in the verification_plan.ods file in this directory.
Testbench should always raise a simulation succeed flag but when an error is being detected, sva_checker module will stop the simulation and write an error message.

You can turn that error message to a warning message which will not stop the simulation by modifiying the following line in sva_checker.sv :

`define assert_prog_clk( arg ) \
    assert property (@(posedge prog_clk) disable iff (pReset || (enable_assertions == 1'b0)) arg ) else $error("Simulation failed");

becomes 

`define assert_prog_clk( arg ) \
    assert property (@(posedge prog_clk) disable iff (pReset || (enable_assertions == 1'b0)) arg ) else $warning("Simulation failed");






