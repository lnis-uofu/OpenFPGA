Yosys
=========

*Information taken from Yosys' GitHub*
You need a C++ compiler with C++11 support (up-to-date CLANG or GCC is recommended) and some standard tools such as GNU Flex, GNU Bison, and GNU Make. TCL, readline and libffi are optional (see ENABLE_* settings in Makefile). Xdot (graphviz) is used by the show command in yosys to display schematics.

ABC
=========

ABC depends on gcc-4.9. It is precisely this version which is required. If another gcc is used, the compilation will not be finished correctly.

ACE2
=========

ACE2 only needs a compiler to work. Gcc is the one chosen in this case. No issue was ever reported with ACE2 so if you have one, raise an issue so that we can modify it here.
