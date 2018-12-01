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

# Global Variants
# input Option Hash
my %opt_h; 
my $opt_ptr = \%opt_h;

my $CONF_HANDLE;
my ($SCRIPTS_PATH, $CONFIG_FILEPATH, $FPGA_FLOW_PATH);

# !!! this script is called in the parent folder: fpga_flow. If you use the script in the scripts folder it is not going to work! !!!
$FPGA_FLOW_PATH = getcwd();
$SCRIPTS_PATH = "${FPGA_FLOW_PATH}/scripts";
$CONFIG_FILEPATH = "${FPGA_FLOW_PATH}/configs/fpga_spice/k6_N10_sram_tsmc40nm_TT.conf";

sub spot_option($ $) {
  my ($start,$target) = @_;
  my ($arg_no,$flag) = (-1,"unfound");
  for (my $iarg = $start; $iarg < $#ARGV+1; $iarg++) {
    if ($ARGV[$iarg] eq $target) {
      if ("found" eq $flag) {
        print "Error: Repeated Arguments!(IndexA: $arg_no,IndexB: $iarg)\n";
        &print_usage();        
      } else {
        $flag = "found";
        $arg_no = $iarg;
      }
    }
  }
  # return the arg_no if target is found
  # or return -1 when target is missing
  return $arg_no; 
}

# Specify in the input list,
# 1. Option Name
# 2. Whether Option with value. if yes, choose "on"
# 3. Whether Option is mandatory. If yes, choose "on"
sub read_opt_into_hash($ $ $) {
  my ($opt_name,$opt_with_val,$mandatory) = @_;
  # Check the -$opt_name
  my ($opt_fact) = ("-".$opt_name);
  my ($cur_arg) = (0);
  my ($argfd) = (&spot_option($cur_arg,"$opt_fact"));
  if ($opt_with_val eq "on") {
    if (-1 != $argfd) {
      if ($ARGV[$argfd+1] =~ m/^-/) {
        print "The next argument cannot start with '-'!\n"; 
        print "it implies an option!\n";
      } else {
        $opt_ptr->{"$opt_name\_val"} = $ARGV[$argfd+1];
        $opt_ptr->{"$opt_name"} = "on";
      }     
    } else {
      $opt_ptr->{"$opt_name"} = "off";
      if ($mandatory eq "on") {
        print "Mandatory option: $opt_fact is missing!\n";
        &print_usage();
      }
    }
  } else {
    if (-1 != $argfd) {
      $opt_ptr->{"$opt_name"} = "on";
    }
    else {
      $opt_ptr->{"$opt_name"} = "off";
      if ($mandatory eq "on") {
        print "Mandatory option: $opt_fact is missing!\n";
        &print_usage();
      }
    }  
  }
  return 1;
}

# Read options
sub opts_read() {
  # if no arguments detected, print the usage.
  if (-1 == $#ARGV) {
    print "Error : No input arguments!\n";
    print "Help desk:\n";
    &print_usage();
    exit(1);
  }
  # Read in the options
  my ($cur_arg,$arg_found);
  $cur_arg = 0;
  print "Analyzing your options...\n";
  # Read the options with internal options
  my $argfd;
  # Check help fist 
  $argfd = &spot_option($cur_arg,"-help");
  if (-1 != $argfd) {
    print "Help desk:\n";
    &print_usage();
  }  
  # Then Check the debug with highest priority
  $argfd = &spot_option($cur_arg,"-debug");
  if (-1 != $argfd) {
    $opt_ptr->{"debug"} = "on";
  }
  else {
    $opt_ptr->{"debug"} = "off";
  }
  # Check mandatory options
  # Check the -conf
  # Read Opt into Hash(opt_ptr) : "opt_name","with_val","mandatory"
  &read_opt_into_hash("output_conf","on","on");
  &read_opt_into_hash("arch","on","on");
  &read_opt_into_hash("benchmark_path","on","on");
  &read_opt_into_hash("flow_type","on","on");
  &read_opt_into_hash("power_property_xml","on","on");

  &print_opts(); 

  return 1;
}
  
# List the options
sub print_opts() {
  print "List your options\n"; 
  
  while(my ($key,$value) = each(%opt_h)) {
    print "$key : $value\n";
  }

  return 1;
}

sub print_usage() {
  print "generate configuration file for FPGA flow.\n";
  print "Usage: perl generate_config.pl <options>\n";
  print "       Mandatory options:\n"; 
  print "       -output_conf <string>: specify the path of configuration file to be outputted\n";
  print "       -arch <string>: specify the architecture file\n";
  print "       -benchmark_path <string>: specify the path of benchmark files\n";
  print "       -flow_type <string> : specify the type of FPGA flow to run\n";
  print "       -power_property_xml <string> : specify the XML file containing power property of FPGA architectures\n";
  return 1;
}

# Create paths if it does not exist.
sub generate_path($) {
  my ($mypath) = @_; 
  if (!(-e "$mypath")) {
    mkpath "$mypath";
    print "Path($mypath) does not exist...Create it.\n";
  }
  return 1;
}

# Opens the file in order to write into it 
sub open_file($) {
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
  print $CONF_HANDLE "benchmark_dir = $opt_ptr->{benchmark_path_val}\n";
  print $CONF_HANDLE "yosys_path = ${FPGA_FLOW_PATH}/../yosys/yosys\n";
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
  print $CONF_HANDLE "flow_type = $opt_ptr->{flow_type_val} #standard|mpack2|mpack1|vtr_standard|vtr|yosys_vpr\n";
  print $CONF_HANDLE "vpr_arch = $opt_ptr->{arch_val} # Use relative path under VPR folder is OK\n";
  print $CONF_HANDLE "mpack1_abc_stdlib = DRLC7T_SiNWFET.genlib # Use relative path under ABC folder is OK\n";
  print $CONF_HANDLE "m2net_conf = ${FPGA_FLOW_PATH}/m2net_conf/m2x2_SiNWFET.conf\n";
  print $CONF_HANDLE "mpack2_arch = K6_pattern7_I24.arch\n";
  print $CONF_HANDLE "power_tech_xml = $opt_ptr->{power_property_xml_val} # Use relative path under VPR folder is OK\n";
  print $CONF_HANDLE "\n";
  print $CONF_HANDLE "[csv_tags]\n";
  print $CONF_HANDLE "mpack1_tags = Global mapping efficiency:|efficiency:|occupancy wo buf:|efficiency wo buf:\n";
  print $CONF_HANDLE "mpack2_tags = BLE Number:|BLE Fill Rate: \n";
  print $CONF_HANDLE "vpr_tags = Netlist clb blocks:|Final critical path:|Total logic delay:|total net delay:|Total routing area:|Total used logic block area:|Total wirelength:|Packing took|Placement took|Routing took|Average net density:|Median net density:|Recommend no. of clock cycles:\n";
  print $CONF_HANDLE "vpr_power_tags = PB Types|Routing|Switch Box|Connection Box|Primitives|Interc Structures|lut6|ff\n";
  return 1;
}

# Closes the file after being used
sub close_file($) {
  close ($CONF_HANDLE) || warn "close failed: $!";
  return 1;
}

# Input program path is like "~/program_dir/program_name"
# We split it from the scalar
sub split_prog_path($) { 
  my ($prog_path) = @_;
  my @path_elements = split /\//,$prog_path;
  my ($prog_dir,$prog_name);
 
  $prog_name = $path_elements[$#path_elements]; 
  $prog_dir = $prog_path;
  $prog_dir =~ s/$prog_name$//g;
    
  return ($prog_dir,$prog_name);
}


# Main routine
sub main() {
  &opts_read();

  $CONFIG_FILEPATH = $opt_ptr->{output_conf_val};

  my ($CONFIG_DIR_PATH, $CONFIG_FILENAME) =  &split_prog_path($CONFIG_FILEPATH); 

  &generate_path($CONFIG_DIR_PATH);
  &open_file($CONFIG_FILEPATH);
  &generate_file($CONFIG_FILEPATH);
  &close_file($CONFIG_FILEPATH);

  print "Configuration file $CONFIG_FILEPATH generated!\n";

  return 1;  
}

&main();
exit(0);


