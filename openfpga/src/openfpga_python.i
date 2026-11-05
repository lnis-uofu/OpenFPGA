/* SWIG interface file for OpenFPGA shell APIs (Python) */
%module openfpga

%{
#include "openfpga_shell.h"
%}

/* start() takes a C-style argv, which is not usable from Python */
%ignore OpenfpgaShell::start;

%include "openfpga_shell.h"
