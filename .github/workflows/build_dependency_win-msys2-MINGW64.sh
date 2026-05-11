#!/usr/bin/env bash

# Build dependencies for MSYS2 (Windows)
# Converted from Ubuntu 22.04 apt-get script
# Run this script inside an MSYS2 MINGW64 shell (or UCRT64)

# Update package database
# Latest gcc-16 is not yet stable. Pacman can only install the rolling version. So comment it out
#pacman -Syu --noconfirm
#    mingw-w64-x86_64-toolchain \

# This is for code compilation
pacman -S --noconfirm --needed \
    autoconf \
    automake \
    libtool \
    bison \
    ccache \
    base-devel \
    mingw-w64-x86_64-autotools \
    mingw-w64-x86_64-cmake \
    mingw-w64-x86_64-ctags \
    curl \
    mingw-w64-x86_64-doxygen \
    flex \
    mingw-w64-x86_64-fontconfig \
    gdb \
    git \
    gperf \
    mingw-w64-x86_64-iverilog \
    mingw-w64-x86_64-cairo \
    mingw-w64-x86_64-libevent \
    mingw-w64-x86_64-libffi \
    mingw-w64-x86_64-fontconfig \
    ncurses-devel \
    mingw-w64-x86_64-readline \
    mingw-w64-x86_64-libxml2 \
    mingw-w64-x86_64-make \
    perl \
    mingw-w64-x86_64-pkg-config \
    mingw-w64-x86_64-python3 \
    mingw-w64-x86_64-python \
    mingw-w64-x86_64-python-pip \
    mingw-w64-x86_64-python-setuptools \
    mingw-w64-x86_64-flexdll \
    mingw-w64-x86_64-qt5-base \
    mingw-w64-x86_64-tcl \
    mingw-w64-x86_64-tcllib \
    texinfo \
    wget \
    zip \
    mingw-w64-x86_64-swig \
    expect \
    mingw-w64-x86_64-lld \
    mingw-w64-x86_64-clang \
    mingw-w64-x86_64-yosys \
    mingw-w64-x86_64-boost-libs \
    libreadline-devel \
    zlib-devel \
    openssl-devel \
    mingw-w64-x86_64-zlib \
    mingw-w64-x86_64-graphviz \
    mingw-w64-x86_64-qt5 \
    mingw-w64-x86_64-qt-installer-framework \
    mingw-w64-x86_64-capnproto \
    mingw-w64-x86_64-openssl

# This is for openfpga regression tests
pacman -S --noconfirm --needed \
    mingw-w64-x86_64-gtkwave \
    mingw-w64-x86_64-iverilog

## Yosys is still based on this version of gcc
## Install fixed GCC 15.0.1 tools from MSYS2 archive
#pacman -U --noconfirm \
#    https://repo.msys2.org/mingw/mingw64/mingw-w64-x86_64-ccmake-3.30.2-1-any.pkg.tar.zst \
#    https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-gcc-15.2.0-14-any.pkg.tar.zst \
#    https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-gcc-ada-15.2.0-14-any.pkg.tar.zst \
#    https://mirror.msys2.org/mingw/mingw64/mingw-w64-x86_64-gcc-libs-15.2.0-14-any.pkg.tar.zst
# 
## Prevent pacman from upgrading GCC tools beyond 15.0.1
## Add to /etc/pacman.conf IgnorePkg line if not already present
#if grep -q "^IgnorePkg" /etc/pacman.conf; then
#    sed -i 's/^IgnorePkg\s*=\s*/IgnorePkg = mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-gcc-ada mingw-w64-x86_64-gcc-libs /' /etc/pacman.conf
#else
#    echo "IgnorePkg = mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-gcc-ada mingw-w64-x86_64-gcc-libs" >> /etc/pacman.conf
#fi
