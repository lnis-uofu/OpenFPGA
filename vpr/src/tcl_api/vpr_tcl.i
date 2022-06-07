/* 
 *  vpr.i
 */
%module vpr
%include "std_string.i"

%{
  #include "vpr_tcl_api.h"
%}

%include "vpr_tcl_api.h"
