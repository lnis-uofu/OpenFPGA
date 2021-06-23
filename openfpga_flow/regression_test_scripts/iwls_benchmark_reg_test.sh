#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "IWLS'05 benchmark regression tests";
run-task benchmark_sweep/iwls2005 --debug --show_thread_logs
# Run a quick but relaxed QoR check for heterogeneous blocks
#python3 openfpga_flow/scripts/check_qor.py --reference_csv_file openfpga_flow/tasks/benchmark_sweep/vtr_benchmarks/config/vtr_benchmark_golden_results.csv --check_csv_file openfpga_flow/tasks/benchmark_sweep/vtr_benchmarks/latest/task_result.csv --metric_checklist_csv_file openfpga_flow/tasks/benchmark_sweep/vtr_benchmarks/config/metric_checklist.csv --check_tolerance 0.2,100
python3 openfpga_flow/scripts/check_qor.py --reference_csv_file openfpga_flow/tasks/benchmark_sweep/iwls2005/config/iwls_benchmark_golden_results.csv --check_csv_file openfpga_flow/tasks/benchmark_sweep/iwls2005/latest/task_result.csv --metric_checklist_csv_file openfpga_flow/tasks/benchmark_sweep/iwls2005/config/metric_checklist.csv --check_tolerance 0.2,100
