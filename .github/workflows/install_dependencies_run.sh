apt-get install --no-install-recommends -y \
libdatetime-perl libc6 libffi6 libgcc1 libreadline7 libstdc++6 \
libtcl8.6 python3.8 python3-pip zlib1g libbz2-1.0 \
iverilog git rsync make curl

curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py
python3.8 get-pip.py
rm get-pip.py