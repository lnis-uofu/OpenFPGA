FROM ubuntu:16.04

RUN apt-get update -qq -y 
RUN apt-get -y install python3 python3-dev tcl tcl8.6-dev gawk libreadline-dev 

RUN apt-get -y install autoconf automake bison build-essential cmake ctags curl doxygen flex fontconfig g++-4.9 gcc-4.9 gdb git gtkwave gperf iverilog libffi-dev libcairo2-dev libevent-dev libfontconfig1-dev liblist-moreutils-perl libncurses5-dev libx11-dev libxft-dev libxml++2.6-dev perl texinfo time valgrind zip qt5-default

RUN git clone https://github.com/LNIS-Projects/OpenFPGA.git

RUN cd OpenFPGA

RUN echo "mkdir -p build && build" >> build.sh
RUN echo "cmake .. -DCMAKE_BUILD_TYPE=debug" >> build.sh
RUN echo "make" >> build.sh
RUN chmod +x build.sh
RUN build.sh
