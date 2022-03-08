/* File : openfpga.i */
%module openfpga
%include "std_string.i"
//%include "std_vector.i"

%{

void openfpga_api(std::string opt);

%}

void openfpga_api(std::string opt);