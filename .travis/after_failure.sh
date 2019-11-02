#!/bin/bash

source .travis/common.sh
set -e

# Close the after_success.1 fold travis has created already.
travis_time_finish
travis_fold end after_failure.1

start_section "failure.tail" "${RED}Failure output...${NC}"
tail -n 1000 output.log
echo "Failed uploading files to LNIS Server"
scp -qCr $TRAVIS_BUILD_DIR/openfpga_flow/tasks/ u1249762@lab1-1.eng.utah.edu:/var/tmp/travis_bc/$TRAVIS_JOB_ID/
scp output.log u1249762@lab1-1.eng.utah.edu:/var/tmp/travis_bc/$TRAVIS_JOB_ID/
end_section "failure.tail"
