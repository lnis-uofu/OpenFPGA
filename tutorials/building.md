# How to build?

## Dependencies
OpenFPGA requires all the following dependencies:
- autoconf
- automake
- bash
- bison
- build-essential
- cmake (version 3.X at least)
- ctags
- curl
- doxygen
- flex
- fontconfig
- g++-8
- gcc-8
- g++-4.9
- gcc-4.9
- gdb
- git
- gperf
- iverilog
- libcairo2-dev
- libevent-dev
- libfontconfig1-dev
- liblist-moreutils-perl
- libncurses5-dev
- libx11-dev
- libxft-dev
- libxml++2.6-dev
- perl
- python
- texinfo
- time
- valgrind
- zip
- qt5-default

## Docker
If some of these dependencies are not installed on your machine, you can choose to use a Docker (the Docker tool needs to be installed). For the ease of the customer first experience, a Dockerfile is provided in the OpenFPGA folder. A container ready to use can be created with the following command:
- docker run lnis/open_fpga:release <br />
*Warning: This command is for quick testing. If you want to conserve your work, you should certainly use other options, such as "-v".*

Otherwise, a container where you can build OpenFPGA yourself can be created with the following commands:
- docker build . -t open_fpga
- docker run -it --rm -v $PWD:/localfile/OpenFPGA -w="/localfile/OpenFPGA" open_fpga bash<br />
[*docker download link*](https://www.docker.com/products/docker-desktop)

## Building
To build the tool, go in the OpenFPGA folder and do:
- mkdir build && cd build
- cmake .. -DCMAKE_BUILD_TYPE=debug
- make (*WARNING using docker you cannot use "make -j", errors will happen*)
