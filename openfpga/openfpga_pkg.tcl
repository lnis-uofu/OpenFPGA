# export TCLLIBPATH=$(pwd)/openfpga
# Variable for the path of the script
set home [file join [pwd] [file dirname [info script]]]
#Load the shared library of OpenFPGA
catch { load $home/libopenfpga[info sharedlibextension] openfpga}
# create OpenFPGA API cpp classe object
openfpga_api tool
# function to be called with command
proc OPENFPGA { args } {
   if { ( $args eq "" ) } {
            puts "Error: supply at least one command like OPENFPGA version"
         } else {
   tool call_openfpga_shell "$args"
   }
}
# print title
OPENFPGA "call_title"
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
package require Tcl 8.6