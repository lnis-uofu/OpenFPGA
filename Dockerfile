FROM ubuntu:18.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y clang \
    bison \
    flex \
    libreadline-dev \
    gawk \
    tcl-dev \
    libffi-dev \
    git \
    pkg-config \
    python3

RUN apt-get -y install autoconf automake bison build-essential cmake ctags curl doxygen flex fontconfig gdb git gtkwave gperf iverilog libffi-dev libcairo2-dev libevent-dev libfontconfig1-dev liblist-moreutils-perl libncurses5-dev libx11-dev libxft-dev libxml++2.6-dev perl texinfo time valgrind zip qt5-default

RUN mkdir -p /release /dev

RUN cd release && git clone --single-branch --branch master https://github.com/LNIS-Projects/OpenFPGA.git OpenFPGA

RUN cd /release/OpenFPGA && mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=debug -DCMAKE_NO_GRAPHICS=on && make

RUN rm -rf /var/lib/apt/lists/*

WORKDIR /release/OpenFPGA
