# Update as required by some packages
apt-get update
apt-get install --no-install-recommends -y \
libdatetime-perl libc6 libffi-dev libgcc1 libreadline8 libstdc++6 \
libtcl8.6 tcl python3-pip zlib1g libbz2-1.0 \
iverilog git rsync make curl wget tree

# Install Ubuntu 20.04 packages
if [ "$(lsb_release -rs)" = "20.04" ]; then
    apt-get install --no-install-recommends -y \
        python3.8 \
        python3.8-venv
# Install Ubuntu 22.04 packages
#elif [ "$(lsb_release -rs)" = "22.04" ]; then
fi
