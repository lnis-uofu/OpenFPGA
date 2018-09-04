# Make sure a clear start

# Sweep Corner Cases 
set corner_list = (TT)
#set corner_list = (TT FF SS MC)

foreach j ($corner_list)
  #rm -rf ./results
  cd ./scripts

  if ($j == MC) then 
    set mc_opt = (-monte_carlo detail_rpt) 
  else 
    set mc_opt = ()
  endif

  perl fpga_flow.pl -conf ../configs/fpga_spice/k6_N10_sram_tsmc40nm_$j\.conf -benchmark ../benchmarks/fpga_spice_bench.txt -rpt ../csv_rpts/fpga_spice/k6_N10_sram_tsmc40nm_bench_$j\.csv -N 10 -K 6 -power -remove_designs -multi_thread 1 -vpr_fpga_spice ../vpr_fpga_spice_task_lists/k6_N10_sram_tsmc40nm -vpr_fpga_spice_rename_illegal_port -vpr_fpga_spice_sim_mt_num 16 -vpr_fpga_spice_print_top_tb -vpr_fpga_spice_print_component_tb -vpr_fpga_spice_print_grid_tb  #-vpr_fpga_spice_parasitic_net_estimation_off #-vpr_fpga_spice_leakage_only 

  perl run_fpga_spice.pl -conf ../vpr_fpga_spice_conf/sample.conf -task ../vpr_fpga_spice_task_lists/k6_N10_sram_tsmc40nm_standard.txt -rpt ../vpr_fpga_spice_csv_rpts/k6_N10_sram_tsmc40_spice_bench_$j\.csv $mc_opt -parse_top_tb -multi_thread 6 -parse_pb_mux_tb -parse_cb_mux_tb -parse_sb_mux_tb -parse_lut_tb -parse_hardlogic_tb -parse_grid_tb -parse_cb_tb -parse_sb_tb 

  cd ..

end
