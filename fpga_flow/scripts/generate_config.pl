#!usr/bin/perl -w
use strict;
#use Shell;
#Use the time
use Time::gmtime;

#Get Date
my $mydate = gmctime();

use File::Path;
use Cwd;
use FileHandle;
my $CONF_HANDLE;
my ($SCRIPTS_PATH, $CONFIG_PATH, $FPGA_FLOW_PATH);

# !!! this script is called in the parent folder: fpga_flow. If you use the script in the scripts folder it is not going to work! !!!
$FPGA_FLOW_PATH = getcwd();
$SCRIPTS_PATH = "${FPGA_FLOW_PATH}/scripts";
$CONFIG_PATH = "${FPGA_FLOW_PATH}/configs/fpga_spice/k6_N10_sram_tsmc40nm_TT.conf";

sub print_usage()
{
	print "\nThe configuration file is being generated. \nThe output is placed in ../configs/fpga_spice/k6_N10_sram_tsmc40nm_TT.conf\n"; 
return 1;
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

# Opens the file in order to write into it 
sub open_file($)
{
	my ($mypath) = @_;
	open ($CONF_HANDLE, "> $mypath") or die "Can't open $mypath: $!";  
	return 1;	
}

# Generates the content of the configuration file
sub generate_file($)
{
	my ($my_path) = @_;
	print $CONF_HANDLE "# Standard Configuration Example\n";
	print $CONF_HANDLE "[dir_path]\n";
	print $CONF_HANDLE "script_base = $FPGA_FLOW_PATH/scripts/\n";
	print $CONF_HANDLE "benchmark_dir = ${FPGA_FLOW_PATH}/benchmarks/FPGA_SPICE_bench\n";
	print $CONF_HANDLE "odin2_path = ${FPGA_FLOW_PATH}/not_used_atm/odin2.exe\n";
	print $CONF_HANDLE "cirkit_path = ${FPGA_FLOW_PATH}/not_used_atm/cirkit\n";
	print $CONF_HANDLE "abc_path = ${FPGA_FLOW_PATH}/../abc/abc\n";
	print $CONF_HANDLE "abc_mccl_path = ${FPGA_FLOW_PATH}/../abc_with_bb_support/abc\n";
	print $CONF_HANDLE "abc_with_bb_support_path = ${FPGA_FLOW_PATH}/../abc_with_bb_support/abc\n";
	print $CONF_HANDLE "mpack1_path = ${FPGA_FLOW_PATH}/not_used_atm/mpack1\n";
	print $CONF_HANDLE "m2net_path = ${FPGA_FLOW_PATH}/not_used_atm/m2net\n";
	print $CONF_HANDLE "mpack2_path = ${FPGA_FLOW_PATH}/not_used_atm/mpack2\n";
	print $CONF_HANDLE "vpr_path = ${FPGA_FLOW_PATH}/../vpr7_x2p/vpr/vpr\n";
	print $CONF_HANDLE "rpt_dir = ${FPGA_FLOW_PATH}/results\n";
	print $CONF_HANDLE "ace_path = ${FPGA_FLOW_PATH}/../ace2/ace\n";
	print $CONF_HANDLE "\n";
	print $CONF_HANDLE "[flow_conf]\n";
	print $CONF_HANDLE "flow_type = standard #standard|mpack2|mpack1|vtr_standard|vtr\n";
	print $CONF_HANDLE "vpr_arch = ${FPGA_FLOW_PATH}/arch/fpga_spice/k6_N10_sram_tsmc40nm_TT.xml # Use relative path under VPR folder is OK\n";
	print $CONF_HANDLE "mpack1_abc_stdlib = DRLC7T_SiNWFET.genlib # Use relative path under ABC folder is OK\n";
	print $CONF_HANDLE "m2net_conf = ${FPGA_FLOW_PATH}/m2net_conf/m2x2_SiNWFET.conf\n";
	print $CONF_HANDLE "mpack2_arch = K6_pattern7_I24.arch\n";
	print $CONF_HANDLE "power_tech_xml = ${FPGA_FLOW_PATH}/tech/PTM_45nm/45nm.xml # Use relative path under VPR folder is OK\n";
	print $CONF_HANDLE "\n";
	print $CONF_HANDLE "[csv_tags]\n";
	print $CONF_HANDLE "mpack1_tags = Global mapping efficiency:|efficiency:|occupancy wo buf:|efficiency wo buf:\n";
	print $CONF_HANDLE "mpack2_tags = BLE Number:|BLE Fill Rate: \n";
	print $CONF_HANDLE "vpr_tags = Netlist clb blocks:|Final critical path:|Total logic delay:|total net delay:|Total routing area:|Total used logic block area:|Total wirelength:|Packing took|Placement took|Routing took|Average net density:|Median net density:|Recommend no. of clock cycles:\n";
	print $CONF_HANDLE "vpr_power_tags = PB Types|Routing|Switch Box|Connection Box|Primitives|Interc Structures|lut6|ff\n";
	return 1;
}

# Closes the file after being used
sub close_file($)
{
close ($CONF_HANDLE) || warn "close failed: $!";
return 1;
}

# Main routine
sub main()
{
	&print_usage();
	&generate_path($CONFIG_PATH);
	&open_file($CONFIG_PATH);
	&generate_file($CONFIG_PATH);
	&close_file($CONFIG_PATH);
	return 1;	
}

&main();
exit(0);


