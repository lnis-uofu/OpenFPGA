# export TCLLIBPATH=$(pwd)/openfpga
# Variable for the path of the script
set home [file join [pwd] [file dirname [info script]]]
#Load the shared library
catch { load $home/libopenfpga[info sharedlibextension] openfpga}
# OpenFPGA class have all the command name functions
oo::class create OpenFPGA_API
oo::define OpenFPGA_API {
    method ComndHelp { args } {
       puts $args
    }

    method read_arch { args } {
         if { ( $args eq "" ) || ( $args eq {-help} ) || ( $args eq {-h} ) } {
            return [ my ComndHelp "\nCommand OPENFPGA read_arch usage:\n\n OPENFPGA read_arch -f or --file <path to architecture XML>\n"]
         }
         set comnd read_openfpga_arch
         foreach item $args {
            lappend comnd $item
         }
         #puts "the given arguments are\n$comnd"
         openfpga_api "$comnd"
    }
     method write_arch { args } {
         if { ( $args eq "" ) || ( $args eq {-help} ) || ( $args eq {-h} ) } {
            return [ my ComndHelp "\nCommand OPENFPGA write_arch usage:\n\n OPENFPGA write_arch -f or --file <specify the architecture XML name>\n"]
         }
         set comnd write_openfpga_arch
         foreach item $args {
            lappend comnd $item
         }
         #puts "the given arguments are\n$comnd"
         openfpga_api "$comnd"
    }
}

oo::define OpenFPGA_API {
    constructor {} {
        #puts "called constructor"
        openfpga_api "call_title"
    }
    destructor {  1
        puts "[self] saving OpenFPGA_API data to database"
    }
}

OpenFPGA_API create OPENFPGA
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