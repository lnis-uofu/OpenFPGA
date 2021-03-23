#!/bin/bash

set -e
source openfpga.sh
PYTHON_EXEC=python3.8
###############################################
# OpenFPGA Shell with VPR8
##############################################
echo -e "VTR benchmark regression tests";
run-task benchmark_sweep/vtr_benchmarks --debug --show_thread_logs
# Run a quick QoR check for heterogeneous blocks
python3 openfpga_flow/scripts/check_qor.py --reference_csv_file openfpga_flow/tasks/benchmark_sweep/vtr_benchmarks/config/vtr_benchmark_golden_results.csv --check_csv_file openfpga_flow/tasks/benchmark_sweep/vtr_benchmarks/latest/task_result.csv --metric_checklist_csv_file openfpga_flow/tasks/benchmark_sweep/vtr_benchmarks/config/metric_checklist.csv
