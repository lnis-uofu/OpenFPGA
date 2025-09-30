#!/usr/bin/env bash

# The package list is designed for Ubuntu 20.04 LTS
add-apt-repository -y ppa:ubuntu-toolchain-r/test
apt-get update
apt-get install -y \
    autoconf \
    automake \
    bison \
    ccache \
    cmake \
    curl \
    doxygen \
    libfl-dev \
    flex \
    fontconfig \
    gdb \
    git \
    gperf \
    iverilog \
    libc6-dev \
    libcairo2-dev \
    libevent-dev \
    libffi-dev \
    libfontconfig1-dev \
    liblist-moreutils-perl \
    libncurses5-dev \
    libreadline-dev \
    libreadline8 \
    libx11-dev \
    libxft-dev \
    libxml++2.6-dev \
    make \
    perl \
    pkg-config \
    python3 \
    python3-setuptools \
    python3-lxml \
    python3-pip \
    tcllib \
    tcl8.6-dev \
    texinfo \
    time \
    valgrind \
    wget \
    zip \
    swig \
    expect \
    libxml2-utils

# Install Ubuntu 20.04 packages
if [ "$(lsb_release -rs)" = "20.04" ]; then
    apt-get install -y \
        ctags \
        qt5-default \
        g++-7 \
        gcc-7 \
        g++-8 \
        gcc-8 \
        g++-9 \
        gcc-9 \
        g++-10 \
        gcc-10 \
        g++-11 \
        gcc-11 \
        clang-6.0 \
        clang-7 \
        clang-8 \
        clang-10 \
        clang-format-10 \
        libssl-dev
# Install Ubuntu 22.04 packages
elif [ "$(lsb_release -rs)" = "22.04" ]; then
    apt-get install -y \
        exuberant-ctags \
        qtbase5-dev \
        g++-9 \
        gcc-9 \
        g++-10 \
        gcc-10 \
        g++-11 \
        gcc-11 \
        clang-11 \
        clang-12 \
        clang-13 \
        clang-14 \
        clang-format-14
# Install Ubuntu 24.04 packages
elif [ "$(lsb_release -rs)" = "24.04" ]; then
    apt-get install -y \
        exuberant-ctags \
        qtbase5-dev \
        g++-13 \
        gcc-13 \
        clang-18 \
        clang-format-18
fi
