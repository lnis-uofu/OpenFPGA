# Make sure a clear start
set output_conf = ${PWD}/configs/fpga_spice/k6_N10_sram_tsmc40nm_TT.conf
set benchmark_list = ${PWD}/benchmarks/fpga_spice_bench.txt
set benchmark_path = ${PWD}/benchmarks/FPGA_SPICE_bench/
set arch_file = ${PWD}/arch/fpga_spice/k6_N10_sram_tsmc40nm_TT.xml
set flow_type = standard
set power_property_xml = ${PWD}/tech/PTM_45nm/45nm.xml
set csv_prefix = k6_N10_sram_tsmc40nm
set fpga_spice_flow_config_file = ${PWD}/vpr_fpga_spice_conf/sample.conf
set vpr_flow_report_dir = ${PWD}/csv_rpts/fpga_spice/
set fpga_spice_flow_report_dir = ${PWD}/vpr_fpga_spice_csv_rpts/
set fpga_spice_task_list_dir = ${PWD}/vpr_fpga_spice_task_lists/

# Sweep Corner Cases 
set corner_list = (TT)
#set corner_list = (TT FF SS MC)

foreach j ($corner_list)
  #rm -rf ./results
  if ($j == MC) then 
    set mc_opt = (-monte_carlo detail_rpt) 
  else 
    set mc_opt = ()
  endif

  perl scripts/generate_config.pl -output_conf $output_conf -arch $arch_file -benchmark_path $benchmark_path -flow_type $flow_type -power_property_xml $power_property_xml

  perl scripts/fpga_flow.pl -conf $output_conf -benchmark $benchmark_list -rpt ${vpr_flow_report_dir}${csv_prefix}_$j\.csv -N 10 -K 6 -power -remove_designs -multi_thread 1 -vpr_fpga_spice ${fpga_spice_task_list_dir}${csv_prefix} -vpr_fpga_spice_rename_illegal_port -vpr_fpga_spice_sim_mt_num 16 -vpr_fpga_spice_print_top_tb -vpr_fpga_spice_print_component_tb -vpr_fpga_spice_print_grid_tb  

  perl scripts/run_fpga_spice.pl -conf ${fpga_spice_flow_config_file} -task ${fpga_spice_task_list_dir}${csv_prefix}_${flow_type}.txt -rpt ${fpga_spice_flow_report_dir}${csv_prefix}_$j\.csv $mc_opt -parse_top_tb -multi_thread 2 -parse_pb_mux_tb -parse_cb_mux_tb -parse_sb_mux_tb -parse_lut_tb -parse_hardlogic_tb -parse_grid_tb -parse_cb_tb -parse_sb_tb 

end
