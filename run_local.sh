#!/bin/bash
docker run -it --rm -v "${PWD}":/root/dev/OpenFPGA -w="/root/dev/OpenFPGA" goreganesh/open_fpga bash
pause
