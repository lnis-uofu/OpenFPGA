#############################################
#	Synopsys Design Constraints (SDC)
#	For FPGA fabric 
#	Description: Clock contraints for PnR
#	Author: Xifan TANG 
#	Organization: University of Utah 
#############################################

#############################################
#	Define time unit 
#############################################
set_units -time s

##################################################
# Create programmable clock                       
##################################################
create_clock -name prog_clk[0] -period 9.999999939e-09 -waveform {0 4.99999997e-09} [get_ports {prog_clk[0]}]
##################################################
# Create clock                                    
##################################################
create_clock -name op_clk[0] -period 1.110987968e-09 -waveform {0 5.554939841e-10} [get_ports {op_clk[0]}]
create_clock -name op_clk[1] -period 1.110987968e-09 -waveform {0 5.554939841e-10} [get_ports {op_clk[1]}]
