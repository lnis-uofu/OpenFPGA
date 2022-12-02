#! /bin/env tclsh
# Load dynamic linked library
load "openfpga_shell.so" openfpga_shell
# Create a new Openfpga shell
std::OpenfpgaShell openfpga_shell
# Run an command
openfpga_shell run_command "help"
# Finish the quit
#openfpga_shell run_command "exit"
#exit
