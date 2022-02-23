/* File : openfpga.i */
%module openfpga
%include "std_string.i"
//%include "std_vector.i"

%{
  #include "openfpga_api.h"
%}

%include "openfpga_api.h"

