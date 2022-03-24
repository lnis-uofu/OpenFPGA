# export TCLLIBPATH=$(pwd)/openfpga
# Variable for the path of the script
set home [file join [pwd] [file dirname [info script]]]
#Load the shared library of OpenFPGA
catch { load $home/libopenfpga[info sharedlibextension] openfpga}
# create OpenFPGA API cpp classe object
openfpga_api OPENFPGA

set version 1.0
set testvar "Hello from OpenFPGA"
# Test procedure
proc testfunc {} {
   puts "  I am responding from test function :) "
}

# start TCL readline if in interactive mode
source $home/readline.tcl
if {$::tcl_interactive} {
   TclReadLine::interact
   }

package provide OpenFPGA $version
package require Tcl 8.6