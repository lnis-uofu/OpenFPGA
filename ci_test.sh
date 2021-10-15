#!/bin/bash

export OPENFPGA_PATH=~/rapidsilicon/openfpga_new

for dir in RTL_Benchmark/*; do
 if [ -d "$dir" ]; then
	echo "Looking for top file in $dir/rtl"

	top_file="$(basename "$dir")"

	FILE=$dir/rtl/$top_file.v

	if [ -f "$FILE" ]; then
    	  echo -e "${CYAN}$top_file exists ${ENDCOLOR}"
	  export design_path=${OPENFPGA_PATH}/$FILE
	  export design_top=$top_file
	  echo "design = $design_path"
	  python3 $OPENFPGA_PATH/openfpga_flow/scripts/run_ci_tests.py ci_tests
	else  
	  echo -e "${RED}Top file not found. Make sure design exists${ENDCOLOR}"
	  exit 1
	fi
 fi
done


