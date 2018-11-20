#!usr/bin/perl -w
use strict
#use Shell;
#Use the time
use Time::gmtime;

#Get Date
my $mydate = gmctime();

use Cwd;
my ($FPGA_FLOW_PATH, $DESTINATION);
$FPGA_FLOW_PATH = abs_path();
open($DESTINATION, '>', '../configs/fpga_spice/k6_N10_sram_tsmc40nm_TT.conf')

sub print_usage()
{
	print "This file generates the configuration for the .run_fpga_spice_testbench_study.sh in the parent folder. The output is placed in ../configs/fpga_spice/k6_N10_sram_tsmc40nm_TT.conf\n" 
	print $DESTINATION "# Standard Configuration Example\n";

 



}

# Create paths if it does not exist.
sub generate_path($)
{
  my ($mypath) = @_; 
  if (!(-e "$mypath"))
  {
    mkpath "$mypath";
    print "Path($mypath) does not exist...Create it.\n";
  }
  return 1;
}




close $DESTINATION;
