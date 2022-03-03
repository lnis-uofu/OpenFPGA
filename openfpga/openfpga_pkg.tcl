# export TCLLIBPATH=$(pwd)/openfpga
# Variable for the path of the script
set home [file join [pwd] [file dirname [info script]]]
#Load the shared library
catch { load $home/libopenfpga[info sharedlibextension] openfpga}
# Created OpenFPGA API cpp class object with name OPENFPGA"
openfpga_api OPENFPGA
puts "OpenFPGA shell is initialised"
puts "Use OpenFPGA command like OPENFPGA read_arch -f <path to architecture file>"
# Variables for version and testing
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
package require Tcl 8.0
