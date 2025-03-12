# Install essential packages
yum update -y
yum install -y \
    epel-release \
    vim \
    wget \
    git \
    tar \
    unzip

yum install -y \
    gcc \
    gcc-c++ \
    make \
    cmake \
    autoconf \
    automake \
    libtool \
    flex \
    bison \
    python3 \
    python3-pip \
    tbb \
    yum-utils \
    glibc-locale-source \
    glibc-langpack-en \
    tree \
    procps \
    perl \
    gperf \
    openssl-devel \
    tcl \
    tk \
    tk-devel \
    zlib-devel \
    readline-devel

# Install required gcc version and setup
yum install -y centos-release-scl
yum install -y devtoolset-10

# FIXME: This may only work on CentOS8 or later. Consider to compile from scratch
yum install -y https://dl.fedoraproject.org/pub/epel/8/Modular/x86_64/Packages/s/swig-4.0.2-9.module_el8+12710+6335019d.x86_64.rpm
localedef -i en_US -f UTF-8 en_US.UTF-8

cd /tmp/iverilog \
    && git clone https://github.com/steveicarus/iverilog.git \
    && cd iverilog \    
    && sh autoconf.sh \
    && ./configure &&  make && make install
