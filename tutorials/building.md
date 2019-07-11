# How to build?

## Dependancies

OpenFPGA requires all the dependancies listed below:
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

If all these dependancies are not installed in your machine you can choose to use a Docker (docker tool need to be installed). To ease customer first experience a Dockerfile is provided in OpenFPGA folder. It can be build using the commands: 
- docker build . -t open_fpga
- docker run -it --rm -v $PWD:/localfile/OpenFPGA -w="/localfile/OpenFPGA" open_fpga bash<br />
[*docker download link*](https://www.docker.com/products/docker-desktop)

## Building

To build the tool you have to be in OpenFPGA folder and do:
- mkdir build && cd build
- cmake .. -DCMAKE_BUILD_TYPE=debug
- make OR make -j
