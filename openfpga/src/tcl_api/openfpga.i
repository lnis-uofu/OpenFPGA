/* File : openfpga.i */
%module openfpga
%include "std_string.i"
//%include "std_vector.i"

%ignore "OpenfpgaContext";
%ignore "openfpga_context";
%ignore "openfpga_context.h";
%ignore "shell.h";

%{
  #include "openfpga_api.h"
%}

%include "openfpga_api.h"

