#!usr/bin/perl -w
# use the strict mode
use strict;
# Use the Shell enviornment
#use Shell;
# Use the time
use Time::gmtime;
# Use switch module
#use Switch;
use File::Path;
use Cwd;
use FileHandle;
# Multi-thread support
use threads;
use threads::shared;

# Date
my $mydate = gmctime();
# Current Path
my $cwd = getcwd();

# Global Variants
my ($max_route_width_retry) = (1000);
# input Option Hash
my %opt_h;
my $opt_ptr = \%opt_h;
# configurate file hash
my %conf_h;
my $conf_ptr = \%conf_h;
# reports has
my %rpt_h;
my $rpt_ptr = \%rpt_h;

# Benchmarks
my @benchmark_names;
my %benchmarks;
my $benchmarks_ptr = \%benchmarks;
my $verilog_benchmark;

# Supported flows
my @supported_flows = ("standard",
                       "vtr_mccl",
                       "mccl",
                       "mig_mccl",
                       "mpack2",
                       "mpack1",
                       "vtr",
                       "vtr_standard",
                       "yosys_vpr");
my %selected_flows;

# Configuration file keywords list
# Category for conf file.
# main category : 1st class
my @mctgy;
# sub category : 2nd class
my @sctgy;
# Initialize these categories
@mctgy = ("dir_path",
          "flow_conf",
          "csv_tags",
         );
# refer to the keywords of dir_path
@{$sctgy[0]} = ("script_base",
                "benchmark_dir",
                "yosys_path",
                "odin2_path",
                "cirkit_path",
                "abc_mccl_path",
                "abc_path",
                "abc_with_bb_support_path",
                "mpack1_path",
                "m2net_path",
                "mpack2_path",
                "vpr_path",
                "rpt_dir",
                "ace_path",
               );
# refer to the keywords of flow_type
@{$sctgy[1]} = ("flow_type",
                "vpr_arch",
                "mpack2_arch",
                "m2net_conf",
                "mpack1_abc_stdlib",
                "power_tech_xml",
                );
# refer to the keywords of csv_tags
@{$sctgy[2]} = ("mpack1_tags",
                "mpack2_tags",
                "vpr_tags",
                "vpr_power_tags"
               );

# ----------Subrountines------------#
# Print TABs and strings
sub tab_print($ $ $)
{
  my ($FILE,$str,$num_tab) = @_;
  my ($my_tab) = ("    ");

  for (my $i = 0; $i < $num_tab; $i++) {
    print $FILE "$my_tab";
  }
  print $FILE "$str";
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
  return 0;
}

# Print the usage
sub print_usage()
{
  print "Usage:\n";
  print "      fpga_flow [-options <value>]\n";
  print "      Mandatory options: \n";
  print "      -conf <file> : specify the basic configuration files for fpga_flow\n";
  print "      -benchmark <file> : the configuration file contains benchmark file names\n";
  print "      -rpt <file> : CSV file consists of data\n";
  print "      -N <int> : N-LUT/Matrix\n";
  print "      Other Options:\n";
  print "      [ General ] \n";
  print "      \t-matlab_rpt <data_name> : .m file consists of data compatible to matlab scripts. Specify the data name to be appeared in the script\n";
  print "      \t-I <int> : Number of inputs of a CLB, mandatory when mpack1 flow is chosen\n";
  print "      \t-K <int> : K-LUT, mandatory when standard flow is chosen\n";
  print "      \t-M <int> : M-Matrix, mandatory when mpack1 flow is chosen\n";
  print "      \t-power : run power estimation oriented flow\n";
  print "      \t-black_box_ace: run activity estimation with black box support. It increase the power.\n";
  print "      \t-remove_designs: remove all the old results.\n";
  print "      \t-multi_thread <int>: turn on the mutli-thread mode, specify the number of threads\n";
  print "      \t-multi_task <int>: turn on the mutli-task mode\n";
  print "      \t-parse_results_only : only parse the flow results and write CSV report.\n";
  print "      \t-debug : debug mode\n";
  print "      \t-help : print usage\n";
  print "      [ ODIN II ] \n";
  print "      \t-min_hard_adder_size: min. size of hard adder in carry chain defined in Arch XML.(Default:1)\n";
  print "      \t-mem_size: size of memory, mandatory when VTR/VTR_MCCL/VTR_MIG_MCCL flow is chosen\n";
  print "      \t-odin2_carry_chain_support: turn on the carry_chain support only valid for VTR_MCCL/VTR_MIG_MCCL flow \n";
  print "      [ ABC ] \n";
  print "      \t-abc_scl : run ABC optimization for sequential circuits, mandatory when VTR flow is selected.\n";
  print "      \t-abc_verilog_rewrite : run ABC to convert a blif netlist to a Verilog netlist.\n";
  print "      [ ACE ] \n";
  print "      \t-ace_p <float> : specify the default signal probablity of PIs in ACE2.\n";
  print "      \t-ace_d <float> : specify the default signal density of PIs in ACE2.\n";
  print "      [ VPR - Original Version ] \n";
  print "      \t-vpr_timing_pack_off : turn off the timing-driven pack for vpr.\n";
  print "      \t-vpr_place_clb_pin_remap: turn on place_clb_pin_remap in VPR.\n";
  print "      \t-vpr_max_router_iteration <int> : specify the max router iteration in VPR.\n";
  print "      \t-vpr_route_breadthfirst : use the breadth-first routing algorithm of VPR.\n";
  print "      \t-vpr_use_tileable_route_chan_width: turn on the conversion to tileable_route_chan_width in VPR.\n";
  print "      \t-min_route_chan_width <float> : turn on routing with <float>* min_route_chan_width.\n";
  print "      \t-fix_route_chan_width : turn on routing with a fixed route_chan_width, defined in benchmark configuration file.\n";
  print "      [ VPR - FPGA-X2P Extension ] \n";
  print "      \t-vpr_fpga_x2p_rename_illegal_port : turn on renaming illegal ports option of VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_x2p_signal_density_weight <float>: specify the option signal_density_weight of VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_x2p_sim_window_size <float>: specify the option sim_window_size of VPR FPGA SPICE\n";
  print "      [ VPR - FPGA-SPICE Extension ] \n";
  print "      \t-vpr_fpga_spice <task_file> : turn on SPICE netlists print-out in VPR, specify a task file\n";
  print "      \t-vpr_fpga_spice_sim_mt_num <int>: specify the option sim_mt_num of VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_spice_print_component_tb : print component-level testbenches in VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_spice_print_grid_tb : print Grid-level testbenches in VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_spice_print_top_tb : print full-chip testbench in VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_spice_leakage_only : turn on leakage_only mode in VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_spice_parasitic_net_estimation_off : turn off parasitic_net_estimation in VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_spice_testbench_load_extraction_off : turn off testbench_load_extraction in VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_spice_simulator_path <string> : Specify simulator path\n";
  print "      [ VPR - FPGA-Verilog Extension ] \n";
  print "      \t-vpr_fpga_verilog : turn on Verilog Generator of VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_verilog_dir <verilog_path>: provide the path where generated verilog files will be written\n";
  print "      \t-vpr_fpga_verilog_include_timing : turn on printing delay specification in Verilog files\n";
  print "      \t-vpr_fpga_verilog_include_signal_init : turn on printing signal initialization in Verilog files\n";
  print "      \t-vpr_fpga_verilog_print_autocheck_top_testbench: turn on printing autochecked top-level testbench for Verilog Generator of VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_verilog_formal_verification_top_netlist : turn on printing formal top Verilog files\n";
  print "      \t-vpr_fpga_verilog_include_icarus_simulator : Add syntax and definition required to use Icarus Verilog simulator\n";
  print "      \t-vpr_fpga_verilog_print_user_defined_template : \n";
  print "      \t-vpr_fpga_verilog_print_report_timing_tcl : Generate tcl script useful for timing report generation\n";
  print "      \t-vpr_fpga_verilog_report_timing_rpt_path <path_to_generate_reports> : Specify path for report timing\n";
  print "      \t-vpr_fpga_verilog_print_sdc_pnr : Generate sdc file to constraint Hardware P&R\n";
  print "      \t-vpr_fpga_verilog_print_sdc_analysis : Generate sdc file to do STA\n";
  print "      \t-vpr_fpga_verilog_print_top_tb : turn on printing top-level testbench for Verilog Generator of VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_verilog_print_input_blif_tb : turn on printing testbench for input blif file in Verilog Generator of VPR FPGA SPICE\n";
  print "      \t-vpr_fpga_verilog_print_modelsim_autodeck <modelsim.ini_path>: turn on printing modelsim simulation script\n";
  print "      [ VPR - FPGA-Bitstream Extension ] \n";
  print "      \t-vpr_fpga_bitstream_generator: turn on FPGA-SPICE bitstream generator\n";
  exit(1);
  return 1;
}

sub spot_option($ $)
{
  my ($start,$target) = @_;
  my ($arg_no,$flag) = (-1,"unfound");
  for (my $iarg = $start; $iarg < $#ARGV+1; $iarg++)
  {
    if ($ARGV[$iarg] eq $target)
    {
      if ("found" eq $flag)
      {
        print "Error: Repeated Arguments!(IndexA: $arg_no,IndexB: $iarg)\n";
        &print_usage();
      }
      else
      {
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
sub read_opt_into_hash($ $ $)
{
  my ($opt_name,$opt_with_val,$mandatory) = @_;
  # Check the -$opt_name
  my ($opt_fact) = ("-".$opt_name);
  my ($cur_arg) = (0);
  my ($argfd) = (&spot_option($cur_arg,"$opt_fact"));
  if ($opt_with_val eq "on")
  {
    if (-1 != $argfd)
    {
      if ($ARGV[$argfd+1] =~ m/^-/)
      {
        print "The next argument cannot start with '-'!\n";
        print "it implies an option!\n";
      }
      else
      {
        $opt_ptr->{"$opt_name\_val"} = $ARGV[$argfd+1];
        $opt_ptr->{"$opt_name"} = "on";
      }
    }
    else
    {
      $opt_ptr->{"$opt_name"} = "off";
      if ($mandatory eq "on")
      {
        print "Mandatory option: $opt_fact is missing!\n";
        &print_usage();
      }
    }
  }
  else
  {
    if (-1 != $argfd)
    {
      $opt_ptr->{"$opt_name"} = "on";
    }
    else
    {
      $opt_ptr->{"$opt_name"} = "off";
      if ($mandatory eq "on")
      {
        print "Mandatory option: $opt_fact is missing!\n";
        &print_usage();
      }
    }
  }
  return 0;
}

# Read options
sub opts_read()
{
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
  } else {
    $opt_ptr->{"debug"} = "off";
  }
  # Check mandatory options
  # Check the -conf
  # Read Opt into Hash(opt_ptr) : "opt_name","with_val","mandatory"
  &read_opt_into_hash("conf","on","on");
  &read_opt_into_hash("benchmark","on","on");
  &read_opt_into_hash("rpt","on","on");
  &read_opt_into_hash("matlab_rpt","on","off"); # Add an option to output report file compatible to matlab scripts
  &read_opt_into_hash("N","on","on");
  &read_opt_into_hash("K","on","off");
  &read_opt_into_hash("I","on","off");
  &read_opt_into_hash("M","on","off");
  &read_opt_into_hash("power","off","off");
  &read_opt_into_hash("vpr_place_clb_pin_remap","off","off");
  &read_opt_into_hash("black_box_ace","off","off");
  &read_opt_into_hash("remove_designs","off","off");
  &read_opt_into_hash("abc_scl","off","off");
  &read_opt_into_hash("abc_verilog_rewrite","off","off");
  &read_opt_into_hash("ace_p","on","off");
  &read_opt_into_hash("ace_d","on","off");
  &read_opt_into_hash("vpr_timing_pack_off","off","off");
  &read_opt_into_hash("vpr_route_breadthfirst","off","off");
  &read_opt_into_hash("vpr_use_tileable_route_chan_width","off","off");
  &read_opt_into_hash("min_route_chan_width","on","off");
  &read_opt_into_hash("fix_route_chan_width","off","off");
  &read_opt_into_hash("vpr_max_router_iteration","on","off");
  &read_opt_into_hash("multi_task","on","off");
  &read_opt_into_hash("multi_thread","on","off");
  &read_opt_into_hash("parse_results_only","off","off");

  # VTR/VTR_MCCL/VTR_MIG_MCCL flow options
  # Read Opt into Hash(opt_ptr) : "opt_name","with_val","mandatory"
  &read_opt_into_hash("min_hard_adder_size","on","off");
  &read_opt_into_hash("mem_size","on","off");
  &read_opt_into_hash("odin2_carry_chain_support","off","off");

  # FPGA-SPICE options
  # Read Opt into Hash(opt_ptr) : "opt_name","with_val","mandatory"
  &read_opt_into_hash("vpr_fpga_spice","on","off");
  &read_opt_into_hash("vpr_fpga_x2p_rename_illegal_port","off","off");
  &read_opt_into_hash("vpr_fpga_x2p_signal_density_weight","on","off");
  &read_opt_into_hash("vpr_fpga_x2p_sim_window_size","on","off");
  &read_opt_into_hash("vpr_fpga_spice_sim_mt_num","on","off");
  &read_opt_into_hash("vpr_fpga_spice_print_component_tb","off","off");
  &read_opt_into_hash("vpr_fpga_spice_print_grid_tb","off","off");
  &read_opt_into_hash("vpr_fpga_spice_print_top_tb","off","off");
  &read_opt_into_hash("vpr_fpga_spice_leakage_only","off","off");
  &read_opt_into_hash("vpr_fpga_spice_parasitic_net_estimation_off","off","off");
  &read_opt_into_hash("vpr_fpga_spice_testbench_load_extraction_off","off","off");
  &read_opt_into_hash("vpr_fpga_spice_simulator_path","on","off");

  # FPGA-Verilog options
  # Read Opt into Hash(opt_ptr) : "opt_name","with_val","mandatory"
  &read_opt_into_hash("vpr_fpga_verilog","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_top_tb","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_input_blif_tb","off","off");
  &read_opt_into_hash("vpr_fpga_bitstream_generator","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_autocheck_top_testbench","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_dir","on","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_modelsim_autodeck","on","off");
  &read_opt_into_hash("vpr_fpga_verilog_include_timing","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_include_signal_init","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_formal_verification_top_netlist","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_include_icarus_simulator","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_report_timing_tcl","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_report_timing_rpt_path","on","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_sdc_pnr","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_sdc_analysis","off","off");
  &read_opt_into_hash("vpr_fpga_verilog_print_user_defined_template","off","off");

  # Regression test option
  &read_opt_into_hash("end_flow_with_test","off","off");

  &print_opts();

  return 0;
}

# List the options
sub print_opts()
{
  print "List your options\n";

  while(my ($key,$value) = each(%opt_h))
  {print "$key : $value\n";}

  return 0;
}


# Read each line and ignore the comments which starts with given arg
# return the valid information of line
sub read_line($ $)
{
  my ($line,$com) = @_;
  my @chars;
  if (defined($line))
  {
    @chars = split/$com/,$line;
    if (!($line =~ m/[\w\d]/))
    {$chars[0] = undef;}
    if ($line =~ m/^\s*$com/)
    {$chars[0] = undef;}
  }
  else
  {$chars[0] = undef;}
  if (defined($chars[0]))
  {
    $chars[0] =~ s/^(\s+)//g;
    $chars[0] =~ s/(\s+)$//g;
  }
  return $chars[0];
}

# Check each keywords has been defined in configuration file
sub check_keywords_conf()
{
  for (my $imcg = 0; $imcg<$#mctgy+1; $imcg++)
  {
    for (my $iscg = 0; $iscg<$#{$sctgy[$imcg]}+1; $iscg++)
    {
      if (defined($conf_ptr->{$mctgy[$imcg]}->{$sctgy[$imcg]->[$iscg]}->{val}))
      {
        if ("on" eq $opt_ptr->{debug})
        {
          print "Keyword($mctgy[$imcg],$sctgy[$imcg]->[$iscg]) = ";
          print "$conf_ptr->{$mctgy[$imcg]}->{$sctgy[$imcg]->[$iscg]}->{val}";
          print "\n";
        }
      }
      else
      {die "Error: Keyword($mctgy[$imcg],$sctgy[$imcg]->[$iscg]) is missing!\n";}
    }
  }
  return 0;
}

# Read the configuration file
sub read_conf()
{
  # Read in these key words
  my ($line,$post_line);
  my @equation;
  my $cur = "unknown";
  open (CONF, "< $opt_ptr->{conf_val}") or die "Fail to open $opt_ptr->{conf_val}!\n";
  print "Reading $opt_ptr->{conf_val}...\n";
  while(defined($line = <CONF>))
  {
    chomp $line;
    $post_line = &read_line($line,"#");
    if (defined($post_line))
    {
       if ($post_line =~ m/\[(\w+)\]/)
       {$cur = $1;}
       elsif ("unknown" eq $cur)
       {
         die "Error: Unknown tags for this line!\n$post_line\n";
       }
       else
       {
         $post_line =~ s/\s//g;
         @equation = split /=/,$post_line;
         $conf_ptr->{$cur}->{$equation[0]}->{val} = $equation[1];
       }
    }
  }
  # Check these key words
  print "Read complete!\n";
  &check_keywords_conf();
  print "Checking these keywords...";
  print "Successfully\n";
  close(CONF);
  return 0;
}

sub read_benchmarks()
{
  # Read in file names
  my ($line,$post_line,$cur);
  $cur = 0;
  open (FCONF,"< $opt_ptr->{benchmark_val}") or die "Fail to open $opt_ptr->{benchmark_val}!\n";
  print "Reading $opt_ptr->{benchmark_val}...\n";
  while(defined($line = <FCONF>))
  {
    chomp $line;
    $post_line = &read_line($line,"#");
    if (defined($post_line)) {
      $post_line =~ s/\s+//g;
      my @tokens = split(",",$post_line);
      # first is the benchmark name,
      #the second is the channel width, if applicable
      if ($tokens[0]) {
        $benchmark_names[$cur] = $tokens[0];
      } else {
        die "ERROR: invalid definition for benchmarks!\n";
      }
      $benchmarks_ptr->{"$benchmark_names[$cur]"}->{fix_route_chan_width} = $tokens[1];
      $cur++;
    }
  }
  print "Benchmarks(total $cur):\n";
  foreach my $temp(@benchmark_names)
  {print "$temp\n";}
  close(FCONF);
  return 0;
}

# Input program path is like "~/program_dir/program_name"
# We split it from the scalar
sub split_prog_path($)
{
  my ($prog_path) = @_;
  my @path_elements = split /\//,$prog_path;
  my ($prog_dir,$prog_name);

  $prog_name = $path_elements[$#path_elements];
  $prog_dir = $prog_path;
  $prog_dir =~ s/$prog_name$//g;

  return ($prog_dir,$prog_name);
}

sub check_blif_type($)
{
  my ($blif) = @_;
  my ($line);
  open (BLIF, "< $blif") or die "Fail to open $blif!\n";
  while(defined($line = <BLIF>)) {
    chomp $line;
    if ($line =~ /^\.latch/) {
      close(BLIF);
      return "seq";
    }
  }
  close(BLIF);
  return "comb";
}

# Check Options
sub check_opts() {
  # Task 1: min_chan_width <float> > 1
  if (("on" eq $opt_ptr->{min_route_chan_width})
     &&(1. > $opt_ptr->{min_route_chan_width_val})) {
    die "ERROR: Invalid -min_chan_width, should be at least 1.0!\n";
  }
  # Task 2: check mandatory option when flow mpack1 is chosen
  if ("on" eq $selected_flows{"mpack1"}->{flow_status}) {
    if ("off" eq $opt_ptr->{M}) {
      die "ERROR: Option -M should be specified when flow mpack1 is selected!\n";
    }
    if ("off" eq $opt_ptr->{I}) {
      die "ERROR: Option -I should be specified when flow mpack1 is selected!\n";
    }
  }
  # Task 3: check mandatory options when flow vtr is chosen
  if ("on" eq $selected_flows{"vtr"}->{flow_status}) {
    if ("off" eq $opt_ptr->{mem_size}) {
      die "ERROR: Option -mem_size should be specified when flow vtr is selected\n";
    }
    if ("off" eq $opt_ptr->{K}) {
      die "ERROR: Option -K should be specified when flow vtr is selected\n";
    }
    if ("off" eq $opt_ptr->{abc_scl}) {
      die "ERROR: Option -abc_scl should be specified when flow vtr is selected\n";
    }
  }
  # Task 3: check mandatory options when flow vtr_standard or standard is chosen
  if (("on" eq $selected_flows{"standard"}->{flow_status})
     ||("on" eq $selected_flows{"vtr_standard"}->{flow_status})) {
    if ("off" eq $opt_ptr->{K}) {
      die "ERROR: Option -K should be specified when flow vtr_standard|standard is selected\n";
    }
  }
}

# Run ABC with standard library mapping
sub run_abc_libmap($ $ $)
{
  my ($bm,$blif_out,$log) = @_;
  # Get ABC path
  my ($abc_dir,$abc_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_path}->{val});

  chdir $abc_dir;
  my ($mpack1_stdlib) = ($conf_ptr->{flow_conf}->{mpack1_abc_stdlib}->{val});
  # Run MPACK ABC
  my ($abc_seq_optimize) = ("");
  if (("on" eq $opt_ptr->{abc_scl})&&("seq" eq &check_blif_type($bm))) {
    ($abc_seq_optimize) = ("scl -l;");
  }
  # !!! For standard library, we cannot use sweep ???
  system("./$abc_name -c \"read_blif $bm; resyn2; read_library $mpack1_stdlib; $abc_seq_optimize map -v; write_blif $blif_out; quit;\" > $log");
  chdir $cwd;
}

# Rewrite the verilog after optimization
sub run_rewrite_verilog($ $ $ $ $) {
  my ($blif, $path, $benchmark, $bm, $log) = @_;
  my ($new_verilog) = "$path/$benchmark".".v";
  my ($cmd_log) = ($log);
  $cmd_log =~ s/\.log$/_rewrite_verilog\.ys/;

  # Get Yosys path
  my ($yosys_dir,$yosys_name) = &split_prog_path($conf_ptr->{dir_path}->{yosys_path}->{val});

  print "Entering $yosys_dir\n";
  chdir $yosys_dir;
  my ($lut_num) = $opt_ptr->{K_val};

  # Create yosys synthesize script
  my ($YOSYS_CMD_FH) = (FileHandle->new);
  if ($YOSYS_CMD_FH->open("> $cmd_log")) {
    print "INFO: auto generating cmds for Yosys ($cmd_log) ...\n";
  } else {
    die "ERROR: fail to auto generating cmds for Yosys ($cmd_log) ...\n";
  }
  # Output the standard format (refer to VTR_flow script)
  print $YOSYS_CMD_FH "# Yosys rewriting verilog script for $bm\n";
  print $YOSYS_CMD_FH "read_blif $blif\n";
  print $YOSYS_CMD_FH "write_verilog $new_verilog\n";

  close($YOSYS_CMD_FH);
  #
  # Create a local copy for the commands

  system("./$yosys_name $cmd_log > $log");

  if (!(-e $new_verilog)) {
    die "ERROR: Yosys fail at rewriting benchmark $bm.\n";
  }

  print "Leaving $yosys_dir\n";
  chdir $cwd;

  return ($new_verilog);
}

# Run yosys synthesis with ABC LUT mapping
sub run_yosys_fpgamap($ $ $ $) {
  my ($bm, $bm_path, $blif_out, $log) = @_;
  my ($cmd_log) = ($log);
  $cmd_log =~ s/log$/ys/;

  # Get Yosys path
  my ($yosys_dir,$yosys_name) = &split_prog_path($conf_ptr->{dir_path}->{yosys_path}->{val});

  print "Entering $yosys_dir\n";
  chdir $yosys_dir;
  my ($lut_num) = $opt_ptr->{K_val};

  # Create yosys synthesize script
  my ($YOSYS_CMD_FH) = (FileHandle->new);
  if ($YOSYS_CMD_FH->open("> $cmd_log")) {
    print "INFO: auto generating cmds for Yosys ($cmd_log) ...\n";
  } else {
    die "ERROR: fail to auto generating cmds for Yosys ($cmd_log) ...\n";
  }
  # Output the standard format (refer to VTR_flow script)
  print $YOSYS_CMD_FH "# Yosys synthesis script for $bm\n";
  print $YOSYS_CMD_FH "# read Verilog \n";
  print $YOSYS_CMD_FH "read_verilog -nolatches $bm_path\n";
  print $YOSYS_CMD_FH "\n";

  print $YOSYS_CMD_FH "# Technology mapping\n";
  print $YOSYS_CMD_FH "hierarchy -top $bm\n";
  print $YOSYS_CMD_FH "proc\n";
  print $YOSYS_CMD_FH "techmap -D NO_LUT -map +/adff2dff.v\n";
  print $YOSYS_CMD_FH "\n";

  print $YOSYS_CMD_FH "# Synthesis\n";
  print $YOSYS_CMD_FH "synth -top $bm -flatten\n";
  print $YOSYS_CMD_FH "clean\n";
  print $YOSYS_CMD_FH "\n";

  print $YOSYS_CMD_FH "# LUT mapping \n";
  print $YOSYS_CMD_FH "abc -lut $lut_num\n";
  print $YOSYS_CMD_FH "\n";

  print $YOSYS_CMD_FH "# Check \n";
  print $YOSYS_CMD_FH "synth -run check\n";
  print $YOSYS_CMD_FH "\n";

  print $YOSYS_CMD_FH "# Clean and output blif \n";
  print $YOSYS_CMD_FH "opt_clean -purge\n";
  print $YOSYS_CMD_FH "write_blif $blif_out\n";

  close($YOSYS_CMD_FH);
  #
  # Create a local copy for the commands

  system("./$yosys_name $cmd_log > $log");

  if (!(-e $blif_out)) {
    die "ERROR: Fail Yosys for benchmark $bm.\n";
  }

  print "Leaving $yosys_dir\n";
  chdir $cwd;
}

# Run ABC by FPGA-oriented synthesis
sub run_abc_fpgamap($ $ $)
{
  my ($bm,$blif_out,$log) = @_;
  my ($cmd_log) = ($log."cmd");
  # Get ABC path
  my ($abc_dir,$abc_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_path}->{val});

  print "Entering $abc_dir\n";
  chdir $abc_dir;
  my ($lut_num) = $opt_ptr->{K_val};
  # Before we run this blif, identify it is a combinational or sequential
  my ($abc_seq_optimize) = ("");
  if (("on" eq $opt_ptr->{abc_scl})&&("seq" eq &check_blif_type($bm))) {
    ($abc_seq_optimize) = ("scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;");
  }
  my ($fpga_synthesis_method) = ("if");
  #my ($fpga_synthesis_method) = ("fpga");
  #
  my ($dump_verilog) = ("");
  if ("on" eq $opt_ptr->{abc_verilog_rewrite}) {
    $dump_verilog = "write_verilog $bm.v";
  }
  # Run FPGA ABC
  #`csh -cx './$abc_name -c \"read $bm; resyn2; $fpga_synthesis_method -K $lut_num; $abc_seq_optimize $abc_seq_optimize sweep; write_blif $blif_out; quit\" > $log'`;
  my ($ABC_CMD_FH) = (FileHandle->new);
  if ($ABC_CMD_FH->open("> $cmd_log")) {
    print "INFO: auto generating cmds for ABC ($cmd_log) ...\n";
  } else {
    die "ERROR: fail to auto generating cmds for ABC ($cmd_log) ...\n";
  }
  # Output the standard format (refer to VTR_flow script)
  print $ABC_CMD_FH "read $bm; resyn; resyn2; scleanup; $fpga_synthesis_method -K $lut_num; sweep; $abc_seq_optimize write_blif $blif_out; $dump_verilog; quit\n";

  close($ABC_CMD_FH);
  #
  # Create a local copy for the commands

  system("./$abc_name -F $cmd_log > $log");

  if (!(-e $blif_out)) {
    die "ERROR: Fail ABC for benchmark $bm.\n";
  }

  if (("on" eq $opt_ptr->{abc_verilog_rewrite})&&(!(-e "$bm.v"))) {
    die "ERROR: ABC verilog rewrite failed for benchmark $bm!\n";
  }

  print "Leaving $abc_dir\n";
  chdir $cwd;
}

# Run ABC by FPGA-oriented synthesis
sub run_abc_bb_fpgamap($ $ $) {
  my ($bm,$blif_out,$log) = @_;
  # Get ABC path
  my ($abc_dir,$abc_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_with_bb_support_path}->{val});
  my ($lut_num) = $opt_ptr->{K_val};
  # Before we run this blif, identify it is a combinational or sequential
  my ($abc_seq_optimize) = ("");
  if (("on" eq $opt_ptr->{abc_scl})&&("seq" eq &check_blif_type($bm))) {
    ($abc_seq_optimize) = ("scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;");
  }
  my ($fpga_synthesis_method) = ("if");
  #my ($fpga_synthesis_method) = ("fpga");
  #
  my ($dump_verilog) = ("");
  if ("on" eq $opt_ptr->{abc_verilog_rewrite}) {
    $dump_verilog = "write_verilog $bm.v";
  }

  chdir $abc_dir;
  # Run FPGA ABC
  system("./$abc_name -c \"read $bm; resyn; resyn2; $fpga_synthesis_method -K $lut_num; $abc_seq_optimize sweep; write_hie $bm $blif_out; $dump_verilog; quit;\" > $log");

  if (!(-e $blif_out)) {
    die "ERROR: Fail ABC_with_bb_support for benchmark $bm.\n";
  }

  if (("on" eq $opt_ptr->{abc_verilog_rewrite})&&(!(-e "$bm.v"))) {
    die "ERROR: ABC verilog rewrite failed for benchmark $bm!\n";
  }

  chdir $cwd;
}

# Run ABC Carry-chain premapping by FPGA-oriented synthesis
sub run_abc_mccl_fpgamap($ $ $)
{
  my ($bm,$blif_out,$log) = @_;
  # Get ABC path
  my ($abc_mccl_dir,$abc_mccl_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_mccl_path}->{val});
  my ($abc_bb_dir,$abc_bb_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_with_bb_support_path}->{val});
  my ($lut_num) = $opt_ptr->{K_val};
  # Before we run this blif, identify it is a combinational or sequential
  my ($abc_seq_optimize) = ("");
  if (("on" eq $opt_ptr->{abc_scl})&&("seq" eq &check_blif_type($bm))) {
    ($abc_seq_optimize) = ("scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;");
  }
  my ($fpga_synthesis_method) = ("if");
  #my ($fpga_synthesis_method) = ("fpga");

  # Name the intermediate file
  my ($fadds_blif, $interm_blif) = ($blif_out, $blif_out);
  $fadds_blif =~ s/\.blif$/_fadds.blif/;
  $interm_blif =~ s/\.blif$/_interm.blif/;

  my ($min_chain_length) = (4);
  my ($mccl_opt_A, $mccl_opt_B, $mccl_opt_S) = (3, 3, 2);

  chdir $abc_mccl_dir;
  print "INFO: entering abc_mccl directory: $abc_mccl_dir \n";

  # Run ABC three times:
  # 1st time: run abc_with_mccl: read the $bm and do carry-chain detection
  system("./$abc_mccl_name -c \"read $bm; strash; &get; &fadds -nv -N $min_chain_length; \&getspec; \&put; wfadds $fadds_blif; quit;\" > $log.ccdetect");

  # Repeat chdir for multi-thread supporting!
  chdir $abc_mccl_dir;
  print "INFO: entering abc_mccl directory: $abc_mccl_dir \n";

  # 2nd time: run abc_with_mccl: read the $fadds_blif and do carry-chain LUT premapping
  system("./$abc_mccl_name -c \"read $fadds_blif; resyn; resyn2; mccl -A $mccl_opt_A -B $mccl_opt_B -S $mccl_opt_S -K $lut_num -O 1 -r -o $interm_blif; quit;\" > $log.mccl");

  chdir $abc_bb_dir;
  print "INFO: entering abc_with_bb_support directory: $abc_bb_dir \n";
  # 3rd time: run abc_with_bb_support: read the pre-processed blif and do cleanup and recover
  system("./$abc_bb_name -c \"read $interm_blif; $abc_seq_optimize sweep; write_hie $interm_blif $blif_out; quit;\" > $log");

  if (!(-e $blif_out)) {
    die "ERROR: Fail ABC_mccl_FPGA_mapping for benchmark $bm.\n";
  }

  chdir $cwd;
}

# Run ABC MIG Carry-chain premapping by FPGA-oriented synthesis
sub run_abc_mig_mccl_fpgamap($ $ $)
{
  my ($bm,$blif_out,$log) = @_;
  # Get ABC path
  my ($abc_mig_mccl_dir,$abc_mig_mccl_name) = &split_prog_path($conf_ptr->{dir_path}->{cirkit_path}->{val});
  my ($abc_mccl_dir,$abc_mccl_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_mccl_path}->{val});
  my ($abc_bb_dir,$abc_bb_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_with_bb_support_path}->{val});
  my ($lut_num) = $opt_ptr->{K_val};
  # Before we run this blif, identify it is a combinational or sequential
  my ($abc_seq_optimize) = ("");
  if (("on" eq $opt_ptr->{abc_scl})&&("seq" eq &check_blif_type($bm))) {
    ($abc_seq_optimize) = ("scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;");
  }
  my ($fpga_synthesis_method) = ("if");
  #my ($fpga_synthesis_method) = ("fpga");

  # Name the intermediate file
  my ($fadds_blif, $interm_blif) = ($bm, $bm);
  $fadds_blif =~ s/\.blif$/_fadds.blif/;
  $interm_blif =~ s/\.blif$/_interm.blif/;

  my ($min_chain_length) = (4);
  my ($mccl_opt_A, $mccl_opt_B, $mccl_opt_S) = (3, 3, 2);

  chdir $abc_mccl_dir;
  print "INFO: entering abc_mig_mccl directory: $abc_mccl_dir \n";

  # Run ABC three times:
  # 1st time: run abc_with_mig_mccl: read the $bm and do carry-chain detection
  # TODO: unfinished!!!!
  system("./$abc_mig_mccl_name -c \"readv $bm; chains -C ; quit;\" > $log.ccdetect");

  # Repeat chdir for multi-thread supporting!
  chdir $abc_mccl_dir;
  print "INFO: entering abc_mccl directory: $abc_mccl_dir \n";

  # 2nd time: run abc_with_mccl: read the $fadds_blif and do carry-chain LUT premapping
  system("./$abc_mccl_name -c \"read $fadds_blif; resyn; resyn2; mccl -A $mccl_opt_A -B $mccl_opt_B -S $mccl_opt_S -K $lut_num -O 1 -r -o $interm_blif; quit;\" > $log.mccl");

  chdir $abc_bb_dir;
  print "INFO: entering abc_with_bb_support directory: $abc_bb_dir \n";
  # 3rd time: run abc_with_bb_support: read the pre-processed blif and do cleanup and recover
  system("./$abc_bb_name -c \"read $interm_blif; $abc_seq_optimize sweep; write_hie $interm_blif $blif_out; quit;\" > $log");

  if (!(-e $blif_out)) {
    die "ERROR: Fail ABC_mccl_FPGA_mapping for benchmark $bm.\n";
  }

  chdir $cwd;
}

sub run_mpack1p5($ $ $ $ $)
{
  my ($blif_in,$blif_prefix,$matrix_size,$cell_size,$log) = @_;
  # Get MPACK path
  my ($mpack1_dir,$mpack1_name) = &split_prog_path($conf_ptr->{dir_path}->{mpack1_path}->{val});
  chdir $mpack1_dir;
  # Run MPACK
  system("./$mpack1_name $blif_in $blif_prefix -matrix_depth $matrix_size -matrix_width $matrix_size -cell_size $cell_size > $log");
  chdir $cwd;

}

sub run_mpack2($ $ $ $ $ $ $)
{
  my ($blif_in,$blif_out,$mpack2_arch,$net,$stats,$vpr_arch,$log) = @_;
  # Get MPACK path
  my ($mpack2_dir,$mpack2_name) = &split_prog_path($conf_ptr->{dir_path}->{mpack2_path}->{val});
  chdir $mpack2_dir;
  #my ($ble_arch) = ($conf_ptr->{flow_conf}->{mpack_ble_arch}->{val});
  # Run MPACK
  system("./$mpack2_name -blif $blif_in -mpack_blif $blif_out -net $net -ble_arch $mpack2_arch -stats $stats -vpr_arch $vpr_arch > $log");
  chdir $cwd;
}

# Extract Mpack2 stats
sub extract_mpack2_stats($ $ $)
{
  my ($tag,$bm,$mstats) = @_;
  my ($line);
  my @keywords = split /\|/,$conf_ptr->{csv_tags}->{mpack2_tags}->{val};
  open (MSTATS, "< $mstats") or die "ERROR: Fail to open $mstats!\n";
  while(defined($line = <MSTATS>)) {
    chomp $line;
    $line =~ s/\s//g;
    foreach my $tmp(@keywords) {
      $tmp =~ s/\s//g;
      if ($line =~ m/$tmp\s*([0-9E\-\+.]+)/i) {
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$opt_ptr->{K_val}}->{$tmp} = $1;
      }
    }
  }
  close(MSTATS);
}

# Extract Mpack1 stats
sub extract_mpack1_stats($ $ $)
{
  my ($tag,$bm,$mstats) = @_;
  my ($line);
  my @keywords = split /\|/,$conf_ptr->{csv_tags}->{mpack1_tags}->{val};
  open (MSTATS, "< $mstats") or die "ERROR: Fail to open $mstats!\n";
  while(defined($line = <MSTATS>)) {
    chomp $line;
    $line =~ s/\s//g;
    foreach my $tmp(@keywords) {
      $tmp =~ s/\s//g;
      if ($line =~ m/$tmp\s*([0-9E\-\+.]+)/i) {
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$opt_ptr->{M_val}}->{$tmp} = $1;
      }
    }
  }
  close(MSTATS);
}

# Black Box blif for ACE
sub black_box_blif($ $)
{
  my ($blif_in,$blif_out) = @_;
  my ($line);

  open (BF, "< $blif_in") or die "Fail to open $blif_in!\n";
  open (NBF, "> $blif_out") or die "Fail to open $blif_out!\n";
  while(defined($line = <BF>)) {
    chomp $line;
    my @components;
    if ($line =~ m/^\.names/) {
      @components = split /\s+/,$line;
      $line = ".subckt CELL ";
      for (my $i=1; $i < ($opt_ptr->{K_val}+1); $i++) {
        my $i1 = $i - 1;
        if ($i < $#components) {
          $line = $line."I[$i1]=$components[$i] ";
        }
        else {
          $line = $line."I[$i1]=unconn ";
        }
      }
      $line = $line."O[0]=$components[$#components] ";
    }
    print NBF "$line\n";
  }
  # definition of Black box
  print NBF "\n";
  print NBF ".model CELL\n";
  print NBF ".inputs ";
  for (my $i=0; $i < $opt_ptr->{K_val}; $i++) {
    print NBF "I[$i]  ";
  }
  print NBF "\n";
  print NBF ".outputs O[0]\n";
  print NBF ".blackbox\n";
  print NBF ".end\n";
  close(BF);
  close(NBF);
}

# Extract VPR Power Esti
sub extract_vpr_power_esti($ $ $ $)
{
  my ($tag,$ace_vpr_blif,$bm,$type) = @_;
  my ($line,$tmp,$line_num);
  my @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
  my ($vpr_power_stats) = $ace_vpr_blif;

  $line_num = 0;
  $vpr_power_stats =~ s/blif$/power/;
  open (VSTATS, "< $vpr_power_stats") or die "Fail to open $vpr_power_stats!\n";
  while(defined($line = <VSTATS>)) {
    chomp $line;
    $line_num++;
    if ($line =~ m/^Total/i) {
      my @power_info = split  /\s+/,$line;
      if ($#power_info < 3) {
        print "Error: (vpr_power_stats:$vpr_power_stats)ilegal definition at LINE[$line_num]!\n";
        die "Format should be [tag] [Power] [Proposition] [Dynamic Proposition] [Method](Optional)\n";
      }
      if ($power_info[3] > 1) {
        die "Error: (vpr_power_stats:$vpr_power_stats)Dynamic Power Proposition should not be greater than 1 at LINE[$line_num]!\n";
      }
      $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{power}->{total} = $power_info[1];
      $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{power}->{dynamic} = $power_info[1]*$power_info[3];
      $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{power}->{leakage} = $power_info[1]*(1-$power_info[3]);
      next;
    }
    $line =~ s/\s//g;
    foreach my $tmpkw(@keywords) {
      $tmp = $tmpkw;
      $tmp =~ s/\s//g;
      $tmp =~ s/\(/\\\(/g;
      $tmp =~ s/\)/\\\)/g;
      #print "$tmp\n";
      if ($line =~ m/$tmp\s*([0-9E\-+.]+)/i) {
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{power}->{$tmpkw} = $1;
        my @tempdata = split /\./,$rpt_ptr->{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{power}->{$tmpkw};
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{power}->{$tmpkw} = join('.',$tempdata[0],$tempdata[1]);
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{power}->{$tmpkw} =~ s/0$//;
      }
    }
  }
  close(VSTATS);
}

# Extract AAPack stats
sub extract_aapack_stats($ $ $ $ $)
{
  my ($tag,$bm,$vstats,$type,$keywords) = @_;
  my ($line,$tmp);
  open (VSTATS, "< $vstats") or die "Fail to open $vstats!\n";
  while(defined($line = <VSTATS>)) {
    chomp $line;
    #$line =~ s/\s//g;
    foreach my $tmpkw(@{$keywords}) {
      $tmp = $tmpkw;
      $tmp =~ s/\(/\\\(/g;
      $tmp =~ s/\)/\\\)/g;
      if ($line =~ m/\s*([0-9E\-+.]+)\s+of\s+type\s+$tmpkw/i) {
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{$tmpkw} = $1;
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{$tmpkw} =~ s/\.$//;
      }
    }
  }
  close(VSTATS);
}

# Extract min_channel_width VPR stats
sub extract_min_chan_width_vpr_stats($ $ $ $ $ $)
{
  my ($tag,$bm,$vstats,$type,$min_route_chan_width,$parse_results) = @_;
  my ($line,$tmp, $min_chan_width, $chan_width_tag);
  my @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};

  if ("on" eq $min_route_chan_width) {
    $tmp = "Best routing used a channel width factor of";
    $chan_width_tag = "min_route_chan_width";
  } else {
    $tmp = "Circuit successfully routed with a channel width factor of";
    $chan_width_tag = "fix_route_chan_width";
  }
  $tmp =~ s/\s//g;

  open (VSTATS, "< $vstats") or die "ERROR: Fail to open $vstats!\n";
  while(defined($line = <VSTATS>)) {
    chomp $line;
    if (($line =~ m/\s+([0-9]+)\s+of\s+type\s+names/i)
      &&(1 == $parse_results)) {
      $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{LUTs} = $1;
      $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{LUTs} =~ s/\.$//;
    }
    $line =~ s/\s//g;
    if ($line =~ m/$tmp\s*([0-9E\-+.]+)/i) {
      $min_chan_width = $1;
      $min_chan_width =~ s/\.$//;
      if (1 == $parse_results) {
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{$chan_width_tag} = $min_chan_width;
      }
    }
  }
  close(VSTATS);
  return $min_chan_width;
}


# Extract VPR stats
sub extract_vpr_stats($ $ $ $)
{
  my ($tag,$bm,$vstats,$type) = @_;
  my ($line,$tmp);
  my @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
  open (VSTATS, "< $vstats") or die "Fail to open $vstats!\n";
  while(defined($line = <VSTATS>)) {
    chomp $line;
    if ($line =~ m/\s+([0-9]+)\s+of\s+type\s+names/i) {
      $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{LUTs} = $1;
      $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{LUTs} =~ s/\.$//;
    }
    $line =~ s/\s//g;
    foreach my $tmpkw(@keywords) {
      $tmp = $tmpkw;
      $tmp =~ s/\s//g;
      $tmp =~ s/\(/\\\(/g;
      $tmp =~ s/\)/\\\)/g;
      #print "$tmp\n";
      if ($line =~ m/$tmp\s*([0-9E\-+.]+)/i) {
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{$tmpkw} = $1;
        $rpt_h{$tag}->{$bm}->{$opt_ptr->{N_val}}->{$type}->{$tmpkw} =~ s/\.$//;
      }
    }
  }
  close(VSTATS);
}

sub gen_odin2_config_xml($ $ $ $ $ $) {
  my ($config_xml, $odin2_verilog, $odin2_blif_out, $vpr_arch, $mem_size, $min_hard_adder_size) = @_;

  # Open a filehandle
  my ($XMLFH) = (FileHandle->new);
  if ($XMLFH->open("> $config_xml")) {
    print "INFO: auto generating configuration XML for ODIN_II($config_xml) ...\n";
  } else {
    die "ERROR: fail to auto generate configuration XML for ODIN_II($config_xml) ...\n";
  }
  # Output the standard format (refer to VTR_flow script)
  print $XMLFH "<config>\n";
  print $XMLFH "  <verilog_files>\n";
  print $XMLFH "    <verilog_file>$odin2_verilog</verilog_file>\n";
  print $XMLFH "  </verilog_files>\n";
  print $XMLFH "  <output>\n";
  print $XMLFH "    <output_type>blif</output_type>\n";
  print $XMLFH "    <output_path_and_name>$odin2_blif_out</output_path_and_name>\n";
  print $XMLFH "    <target>\n";
  print $XMLFH "      <arch_file>$vpr_arch</arch_file>\n";
  print $XMLFH "    </target>\n";
  print $XMLFH "  </output>\n";
  print $XMLFH "  <optimizations>\n";
  print $XMLFH "    <multiply size=\"3\" fixed=\"1\" fracture=\"0\" padding=\"-1\"/>\n";
  print $XMLFH "    <memory split_memory_width=\"1\" split_memory_depth=\"$mem_size\"/>\n";
  print $XMLFH "    <adder size=\"0\" threshold_size=\"$min_hard_adder_size\"/>\n";
  print $XMLFH "  </optimizations>\n";
  print $XMLFH "  <debug_outputs>\n";
  print $XMLFH "    <debug_output_path>.</debug_output_path>\n";
  print $XMLFH "    <output_ast_graphs>1</output_ast_graphs>\n";
  print $XMLFH "    <output_netlist_graphs>1</output_netlist_graphs>\n";
  print $XMLFH "  </debug_outputs>\n";
  print $XMLFH "</config>\n";

  close($XMLFH);
}

sub run_odin2($ $ $) {
  my ($config_xml, $carry_chain_support, $log) = @_;
  my ($odin2_dir, $odin2_name) = &split_prog_path($conf_ptr->{dir_path}->{odin2_path}->{val});
  my ($options) = ("");

  if ("on" eq $carry_chain_support) {
    $options = $options." -Z";
  }

  chdir $odin2_dir;
  system("./$odin2_name -c $config_xml $options > $log");
  chdir $cwd;
}

sub run_pro_blif_3arg($ $ $) {
  my ($abc_blif_out_bak, $abc_blif_out, $initial_blif) = @_;
  my ($pro_blif_path) = ($conf_ptr->{dir_path}->{script_base}->{val});

  $pro_blif_path =~ s/\/$//g;
  $pro_blif_path = $pro_blif_path . "/pro_blif.pl";

  `perl $pro_blif_path -i $abc_blif_out_bak -o $abc_blif_out -initial_blif $initial_blif`;

  if (!(-e $abc_blif_out)) {
    die "ERROR: Fail pro_blif.pl for benchmark $abc_blif_out.\n";
  }
  return;
}

sub run_pro_blif($ $) {
  my ($abc_blif_out_bak, $abc_blif_out) = @_;
  my ($pro_blif_path) = ($conf_ptr->{dir_path}->{script_base}->{val});

  $pro_blif_path =~ s/\/$//g;
  $pro_blif_path = $pro_blif_path . "/pro_blif.pl";

  `perl $pro_blif_path -i $abc_blif_out_bak -o $abc_blif_out -add_default_clk`;

  if (!(-e $abc_blif_out)) {
    die "ERROR: Fail pro_blif.pl for benchmark $abc_blif_out.\n";
  }
  return;
}

# Run Acitivity Estimation
sub run_ace($ $ $ $) {
  my ($mpack_vpr_blif,$act_file,$ace_new_blif,$log) = @_;
  my ($ace_dir,$ace_name) = &split_prog_path($conf_ptr->{dir_path}->{ace_path}->{val});
  my ($ace_customized_opts) = ("");

  if ("on" eq $opt_ptr->{ace_d}) {
    $ace_customized_opts .= " -d $opt_ptr->{ace_d_val}";
  }

  if ("on" eq $opt_ptr->{ace_p}) {
    $ace_customized_opts .= " -p $opt_ptr->{ace_p_val}";
  }

  print "Entering $ace_dir\n";
  chdir $ace_dir;
  system("./$ace_name -b $mpack_vpr_blif -o $act_file -n $ace_new_blif -c clk $ace_customized_opts >> $log");

  if (!(-e $ace_new_blif)) {
    die "ERROR: Fail ACE for benchmark $mpack_vpr_blif.\n";
  }

  print "Leaving $ace_dir\n";

  chdir $cwd;
}

# Run Icarus Verilog Simulation
sub run_icarus_verilog($ $ $ $ $)
{
  my ($log_file, $compiled_file, $tb_top, $netlists_path, $include_netlists) = @_;

  # Compile and launch simulation
  system("iverilog -o $compiled_file $netlists_path$include_netlists -s $tb_top");
  system("vvp $compiled_file >> $log_file");  # no -j option but could be added to speed-up the process

  # Checking simulation results
  open(F, $log_file);
  my @lines=<F>;
  close F;
  my $keyword = "Succeed";
  my $results = grep($keyword, @lines);
  if($results >= 1){
    print "\nVerification succeed!\n\n";
  } else {
    my $keyword = "Failed";
    my $results = grep($keyword, @lines);
    if($results >= 1){
      print "\nVerification failed\n\n";
    } else {
      die "\nERROR: Simulation didn't start\n\n";
    }
  }
  return;
}

# Run netlists verification using Icarus Simulator
sub run_netlists_verification($)
{
  my ($benchmark) = @_;
  my $log_file = "$benchmark"."_sim.log";
  my $compiled_file = "compiled_"."$benchmark";
  my $include_netlists = "$benchmark"."_include_netlists.v";
  my $tb_top_formal = "$benchmark"."_top_formal_verification_random_tb";
  my $tb_top_autochecked = "$benchmark"."_autocheck_top_tb";
  my $netlists_path = "$opt_ptr->{vpr_fpga_verilog_dir_val}"."/SRC/";

  system("rm -f $log_file");
  system("rm -f $compiled_file");

  if("on" eq $opt_ptr->{vpr_fpga_verilog_include_icarus_simulator}){
    if("on" eq $opt_ptr->{vpr_fpga_verilog_print_autocheck_top_testbench}){
      if("on" eq $opt_ptr->{vpr_fpga_verilog_formal_verification_top_netlist}){ # Preprogramed FPGA netlist chosen if available to speed-up the process
        &run_icarus_verilog($log_file, $compiled_file, $tb_top_formal, $netlists_path, $include_netlists);
      } else {
        &run_icarus_verilog($log_file, $compiled_file, $tb_top_autochecked, $netlists_path, $include_netlists);
      }
    } else {
      die "ERROR: Cannot run netlist verification without \"-vpr_fpga_verilog_print_autocheck_top_testbench\" token.\n";
    }
  } else {
    die "ERROR: Cannot run netlist verification without \"-vpr_fpga_verilog_include_icarus_simulator\" token.\n";
  }
  return;
}

sub run_std_vpr($ $ $ $ $ $ $ $ $)
{
  my ($blif,$bm,$arch,$net,$place,$route,$fix_chan_width,$log,$act_file) = @_;
  my ($vpr_dir,$vpr_name) = &split_prog_path($conf_ptr->{dir_path}->{vpr_path}->{val});
  
  my ($power_opts);
  if ("on" eq $opt_ptr->{power}) {
    $power_opts = "--power --activity_file $act_file --tech_properties $conf_ptr->{flow_conf}->{power_tech_xml}->{val}";
  } else {
    $power_opts = "";
  }
  my ($packer_opts) = ("");
  if ("on" eq $opt_ptr->{vpr_timing_pack_off}) {
    $packer_opts = "--timing_driven_clustering off";
  }

  my ($chan_width_opt) = ("");
  if (($fix_chan_width > 0)||($fix_chan_width == 0)) {
    $chan_width_opt = "-route_chan_width $fix_chan_width";
  }
  if ("on" eq $opt_ptr->{vpr_use_tileable_route_chan_width}) {
    $chan_width_opt = $chan_width_opt." --use_tileable_route_chan_width";
  }

  # FPGA SPICE options
  my ($vpr_spice_opts) = ("");
  if (("on" eq $opt_ptr->{power})&&("on" eq $opt_ptr->{vpr_fpga_spice})) {
    $vpr_spice_opts = "--fpga_spice";

    if ("on" eq $opt_ptr->{vpr_fpga_x2p_signal_density_weight}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_x2p_signal_density_weight $opt_ptr->{vpr_fpga_x2p_signal_density_weight_val}";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_x2p_sim_window_size}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_x2p_sim_window_size $opt_ptr->{vpr_fpga_x2p_sim_window_size_val}";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_sim_mt_num}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_sim_mt_num $opt_ptr->{vpr_fpga_spice_sim_mt_num_val}";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_simulator_path}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_simulator_path $opt_ptr->{vpr_fpga_spice_simulator_path_val}";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_print_component_tb}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_lut_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_hardlogic_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_pb_mux_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_cb_mux_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_sb_mux_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_print_grid_tb}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_grid_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_cb_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_sb_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_print_top_tb}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_print_top_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_leakage_only}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_leakage_only";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_parasitic_net_estimation_off}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_parasitic_net_estimation off";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_testbench_load_extraction_off}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_testbench_load_extraction off";
    }
  }

  # FPGA Verilog options
  if (("on" eq $opt_ptr->{power})&&("on" eq $opt_ptr->{vpr_fpga_verilog})) {
    $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog";

    if ("on" eq $opt_ptr->{vpr_fpga_verilog_dir}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_dir $opt_ptr->{vpr_fpga_verilog_dir_val}";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_top_tb}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_top_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_input_blif_tb}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_input_blif_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_autocheck_top_testbench}) {
      if($verilog_benchmark eq undef){
        $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_autocheck_top_testbench $conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$bm/$bm.v";
      } else {
        $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_autocheck_top_testbench $verilog_benchmark";
      }
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_include_timing}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_include_timing";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_include_signal_init}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_include_signal_init";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_formal_verification_top_netlist}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_formal_verification_top_netlist";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_modelsim_autodeck}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_modelsim_autodeck $opt_ptr->{vpr_fpga_verilog_print_modelsim_autodeck_val}";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_include_icarus_simulator}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_include_icarus_simulator";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_report_timing_tcl}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_report_timing_tcl";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_report_timing_rpt_path}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_report_timing_rpt_path $opt_ptr->{vpr_fpga_verilog_report_timing_rpt_path_val}";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_sdc_pnr}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_sdc_pnr";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_user_defined_template}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_user_defined_template";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_verilog_print_sdc_analysis}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog_print_sdc_analysis";
    }
  }

  # FPGA Bitstream Generator Options
  if ("on" eq $opt_ptr->{vpr_fpga_bitstream_generator}) {
     $vpr_spice_opts = $vpr_spice_opts." --fpga_bitstream_generator";
  }

  if (("on" eq $opt_ptr->{vpr_fpga_x2p_rename_illegal_port})
     || ("on" eq $opt_ptr->{vpr_fpga_spice})
     || ("on" eq $opt_ptr->{vpr_fpga_verilog})) {
    $vpr_spice_opts = $vpr_spice_opts." --fpga_x2p_rename_illegal_port";
  }

  my ($other_opt) = ("");
  if ("on" eq $opt_ptr->{vpr_place_clb_pin_remap}) {
    $other_opt = "--place_clb_pin_remap ";
  }
  if ("on" eq $opt_ptr->{vpr_route_breadthfirst}) {
    $other_opt .= "--router_algorithm breadth_first ";
  }
  if ("on" eq $opt_ptr->{vpr_max_router_iteration}) {
    $other_opt .= "--max_router_iterations $opt_ptr->{vpr_max_router_iteration_val} ";
  }

  chdir $vpr_dir;
  print "Entering $vpr_dir\n";

  print "./$vpr_name $arch $blif --net_file $net --place_file $place --route_file $route --full_stats --nodisp $power_opts $packer_opts $chan_width_opt $vpr_spice_opts $other_opt > $log\n";
  system("./$vpr_name $arch $blif --net_file $net --place_file $place --route_file $route --full_stats --nodisp $power_opts $packer_opts $chan_width_opt $vpr_spice_opts $other_opt > $log");

  #open(F, $log);
  #my @lines=<F>;
  #close F;
  #my @results = grep(" ", @lines);
  #if($#results >= 1){
  #  foreach my $line (0..$#results){
  #    print "$results[$line]\n";
  #  }
  #}
  #if ("on" eq $opt_ptr->{vpr_fpga_verilog_dir}) {
  #  opendir my($dh), $opt_ptr->{vpr_fpga_verilog_dir_val} or die "\nFolder not created!!\n\n";
  #  my @files = readdir $dh;
  #  closedir $dh;
  #  foreach my $file (0..$#files){
  #    print "$files[$file]\t";
  #  }
  #  print "\n";
  #}
  chdir $cwd;
}

sub run_vpr_route($ $ $ $ $ $ $ $ $)
{
  my ($blif,$bm,$arch,$net,$place,$route,$fix_chan_width,$log,$act_file) = @_;
  my ($vpr_dir,$vpr_name) = &split_prog_path($conf_ptr->{dir_path}->{vpr_path}->{val});

  my ($power_opts);
  if ("on" eq $opt_ptr->{power}) {
    $power_opts = "--power --activity_file $act_file --tech_properties $conf_ptr->{flow_conf}->{power_tech_xml}->{val}";
  } else {
    $power_opts = "";
  }

  my ($chan_width_opt) = ("");
  if (($fix_chan_width > 0)||($fix_chan_width == 0)) {
    $chan_width_opt = "--route_chan_width $fix_chan_width";
  }
  if ("on" eq $opt_ptr->{vpr_use_tileable_route_chan_width}) {
    $chan_width_opt = $chan_width_opt." --use_tileable_route_chan_width";
  }

  my ($vpr_spice_opts) = ("");
  if (("on" eq $opt_ptr->{power})&&("on" eq $opt_ptr->{vpr_fpga_spice})) {
    $vpr_spice_opts = "--fpga_spice";
    if ("on" eq $opt_ptr->{vpr_fpga_spice_print_cbsbtb}) {
      $vpr_spice_opts = $vpr_spice_opts." --print_spice_cb_mux_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --print_spice_sb_mux_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_print_pbtb}) {
      $vpr_spice_opts = $vpr_spice_opts." --print_spice_pb_mux_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --print_spice_lut_testbench";
      $vpr_spice_opts = $vpr_spice_opts." --print_spice_hardlogic_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_print_gridtb}) {
      $vpr_spice_opts = $vpr_spice_opts." --print_spice_grid_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_print_toptb}) {
      $vpr_spice_opts = $vpr_spice_opts." --print_spice_top_testbench";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_leakage_only}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_leakage_only";
    }
    if ("on" eq $opt_ptr->{vpr_fpga_spice_parasitic_net_estimation_off}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_spice_parasitic_net_estimation_off";
    }
  }
  if ("on" eq $opt_ptr->{vpr_fpga_verilog}) {
    $vpr_spice_opts = $vpr_spice_opts." --fpga_verilog";
    if ("on" eq $opt_ptr->{vpr_fpga_x2p_rename_illegal_port}) {
      $vpr_spice_opts = $vpr_spice_opts." --fpga_x2p_rename_illegal_port";
    }
  }

  my ($other_opt) = ("");
  if ("on" eq $opt_ptr->{vpr_max_router_iteration}) {
    $other_opt .= "--max_router_iterations $opt_ptr->{vpr_max_router_iteration_val} ";
  }
  if ("on" eq $opt_ptr->{vpr_route_breadthfirst}) {
    $other_opt .= "--router_algorithm breadth_first ";
  }

  chdir $vpr_dir;
  print "Entering $vpr_dir\n";

  print "./$vpr_name $arch $blif --route --blif_file $blif --net_file $net --place_file $place --route_file $route --full_stats --nodisp $power_opts $chan_width_opt $vpr_spice_opts $other_opt > $log\n";
  system("./$vpr_name $arch $blif --route --blif_file $blif --net_file $net --place_file $place --route_file $route --full_stats --nodisp $power_opts $chan_width_opt $vpr_spice_opts $other_opt > $log");
  print "\n";

  chdir $cwd;
}

sub run_mpack1_vpr($ $ $ $ $ $ $)
{
  my ($blif,$arch,$net,$place,$route,$log,$act_file) = @_;
  my ($vpr_dir,$vpr_name) = &split_prog_path($conf_ptr->{dir_path}->{vpr_path}->{val});
  my ($power_opts) = ("");
  if ("on" eq $opt_ptr->{power}) {
    $power_opts = "--power --activity_file $act_file --tech_properties $conf_ptr->{flow_conf}->{power_tech_xml}->{val}";
  }
  chdir $vpr_dir;
  system("./$vpr_name $arch $blif --net_file $net --place_file $place --route_file $route --place --route --full_stats --nodisp $power_opts > $log");
  chdir $cwd;
}

sub run_mpack2_vpr($ $ $ $ $ $ $)
{
  my ($blif,$arch,$net,$place,$route,$min_chan_width,$log) = @_;
  my ($vpr_dir,$vpr_name) = &split_prog_path($conf_ptr->{dir_path}->{vpr_path}->{val});

  my ($power_opts) = ("");
  if ("on" eq $opt_ptr->{power}) {
  #  $power_opts = "--power --activity_file $act_file --tech_properties $conf_ptr->{flow_conf}->{power_tech_xml}->{val}";
  }
  my ($chan_width_opt) = ("");
  if (($min_chan_width > 0)||($min_chan_width == 0)) {
    $min_chan_width = int($min_chan_width*1.2);
    if (0 != $min_chan_width%2) {
      $min_chan_width += 1;
    }
    $chan_width_opt = "-route_chan_width $min_chan_width";
  }

  chdir $vpr_dir;
  system("./$vpr_name $arch $blif --net_file $net --place_file $place --route_file $route --place --route --full_stats --nodisp $power_opts $chan_width_opt > $log");
  chdir $cwd;
}


sub run_aapack($ $ $ $)
{
  my ($blif,$arch,$net,$aapack_log) = @_;
  my ($vpr_dir,$vpr_name) = &split_prog_path($conf_ptr->{dir_path}->{vpr_path}->{val});

  chdir $vpr_dir;

  system("./$vpr_name $arch $blif --net_file $net --pack --timing_analysis off --nodisp > $aapack_log");

  chdir $cwd;
}

sub run_m2net_pack_arch($ $ $ $ $ $)
{
  my ($m2net_conf,$mpack1_rpt,$pack_arch,$N,$I,$m2net_pack_arch_log) = @_;
  my ($m2net_dir,$m2net_name) = &split_prog_path($conf_ptr->{dir_path}->{m2net_path}->{val});

  chdir $m2net_dir;

  system("perl $m2net_name -conf $m2net_conf -mpack1_rpt $mpack1_rpt -mode pack_arch -N $N -I $I -arch_file_pack $pack_arch > $m2net_pack_arch_log");

  chdir $cwd;
}

sub run_m2net_m2net($ $ $ $ $)
{
  my ($m2net_conf,$mpack1_rpt,$aapack_net,$vpr_net,$vpr_arch,$N,$I,$m2net_m2net_log) = @_;
  my ($m2net_dir,$m2net_name) = &split_prog_path($conf_ptr->{dir_path}->{m2net_path}->{val});

  chdir $m2net_dir;

  my ($power_opt) = ("");

  if ("on" eq $opt_ptr->{power}) {
    $power_opt = "-power";
  }

  system("perl $m2net_name -conf $m2net_conf -mpack1_rpt $mpack1_rpt -mode m2net -N $N -I $I -net_file_in $aapack_net -net_file_out $vpr_net -arch_file_vpr $vpr_arch $power_opt > $m2net_m2net_log");

  chdir $cwd;
}

sub run_cirkit_mig_mccl_map($ $ $) {
  my ($bm,$blif_out,$log) = @_;
  my ($bm_aig, $bm_v) = ($blif_out, $blif_out);
  my ($abc_cmd_log, $cirkit_cmd_log) = ($blif_out, $blif_out);

  $bm_aig =~ s/blif$/aig/;
  $bm_v =~ s/blif$/v/;
  $abc_cmd_log =~ s/\.blif$/_abc.cmd/g;
  $cirkit_cmd_log =~ s/\.blif$/_cirkit.cmd/g;

  # Get ABC path
  my ($abc_dir,$abc_name) = &split_prog_path($conf_ptr->{dir_path}->{abc_path}->{val});

  # Get Cirkit path
  my ($cirkit_dir,$cirkit_name) = &split_prog_path($conf_ptr->{dir_path}->{cirkit_path}->{val});
  my ($lut_num) = $opt_ptr->{K_val};

  # Before we run this blif, identify it is a combinational or sequential
  my ($abc_seq_optimize) = ("");
  if (("on" eq $opt_ptr->{abc_scl})&&("seq" eq &check_blif_type($bm))) {
    ($abc_seq_optimize) = ("scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;scleanup;");
  }
  my ($fpga_synthesis_method) = ("if");
  #my ($fpga_synthesis_method) = ("fpga");

  my ($ABC_CMD_FH) = (FileHandle->new);
  if ($ABC_CMD_FH->open("> $abc_cmd_log")) {
    print "INFO: auto generating cmds for ABC ($abc_cmd_log) ...\n";
  } else {
    die "ERROR: fail to auto generating cmds for ABC ($abc_cmd_log) ...\n";
  }
  # Output the standard format (refer to VTR_flow script)
  print $ABC_CMD_FH "read_blif $bm; strash; write $bm_aig; quit;\n";
  close($ABC_CMD_FH);

  # Run ABC to rewrite blif to AIG in verilog format
  chdir $abc_dir;
  system("./$abc_name -F $abc_cmd_log > $log");
  if (!(-e $bm_aig)) {
    die "ERROR: Fail ABC for benchmark $bm.\n";
  }

  my ($CIRKIT_CMD_FH) = (FileHandle->new);
  if ($CIRKIT_CMD_FH->open("> $cirkit_cmd_log")) {
    print "INFO: auto generating cmds for Cirkit ($cirkit_cmd_log) ...\n";
  } else {
    die "ERROR: fail to auto generating cmds for Cirkit ($cirkit_cmd_log) ...\n";
  }
  # Output the standard format (refer to VTR_flow script)
  print $CIRKIT_CMD_FH "read_aiger $bm_aig; xmglut -k 4; write_verilog -x $bm_v; read_verilog -x --as_mig $bm_v; fpga --blif_name $blif_out; quit;\n";
  close($CIRKIT_CMD_FH);

  chdir $cirkit_dir;
  # Run FPGA ABC
  system("./$cirkit_name -f $cirkit_cmd_log >> $log");

  if (!(-e $blif_out)) {
    die "ERROR: Fail Cirkit for benchmark $bm.\n";
  }

  chdir $cwd;
}

sub init_fpga_spice_task($) {
  my ($task_file) = @_;
  my ($task_dir_path, $task_filename) = &split_prog_path($task_file);

  &generate_path($task_dir_path);

  # Open the task file handler
  my ($TASKFH) = (FileHandle->new);
  if ($TASKFH->open("> $task_file")) {
    print "Initializing FPGA SPICE task file($task_file)...\n";
  } else {
    die "ERROR: fail to create task file ($task_file)!\n";
  }

  print $TASKFH "# FPGA SPICE TASKs to run\n";
  print $TASKFH "# Task line format:\n";
  print $TASKFH "# <benchmark_name>,<blif_prefix>,<spice_dir>\n";

  # Close the file handler
  close($TASKFH);
}

# Print a line into task file which contains task info of FPGA SPICE.
sub output_fpga_spice_task($ $ $ $) {
  my ($task_file, $benchmark, $blif_name, $rpt_dir) = @_;
  my ($blif_path, $blif_prefix, $spice_dir);

  # Open the task file handler
  my ($TASKFH) = (FileHandle->new);
  if ($TASKFH->open(">> $task_file")) {
  } else {
    die "ERROR: fail to generate a line for task($benchmark) in task file ($task_file) ...\n";
  }

  ($blif_path,$blif_prefix) = &split_prog_path($blif_name);
  $blif_prefix =~ s/\.blif$//;
  $spice_dir = $rpt_dir;
  $spice_dir =~ s/\/$//;
  $spice_dir = $spice_dir."/spice_netlists/";
  # Output a line
  print $TASKFH "# TaskInfo: $benchmark\n";
  print $TASKFH "$benchmark,$blif_prefix,$spice_dir\n";

  # Close the file handler
  close($TASKFH);
}

sub run_ace_in_flow($ $ $ $ $ $ $) {
  my ($prefix, $abc_blif_out, $act_file,$ace_new_blif,$ace_log) = @_;

  if ("on" eq $opt_ptr->{power}) {
    if ("on" eq $opt_ptr->{black_box_ace}) {
      my ($tmp_blif) = ($prefix."_ace_new.blif");
      &black_box_blif($abc_blif_out,$tmp_blif);
      &run_ace($tmp_blif,$act_file,$ace_new_blif ,$ace_log);
    } else {
      &run_ace($abc_blif_out,$act_file,$ace_new_blif,$ace_log);
    }
    #&run_pro_blif($ace_new_blif, $abc_blif_out);
  }

  if (("on" eq $opt_ptr->{power})&&(!(-e $act_file))) {
    die "ERROR: Fail ACE2 for benchmark $act_file.\n";
  }
}

sub run_vpr_in_flow($ $ $ $ $ $ $ $ $ $ $ $) {
  my ($tag, $benchmark,$benchmark_file, $abc_blif_out, $vpr_arch, $act_file, $vpr_net, $vpr_place, $vpr_route, $vpr_log, $vpr_reroute_log, $parse_results) = @_;

  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    &run_std_vpr($abc_blif_out,$benchmark,$vpr_arch,$vpr_net,$vpr_place,$vpr_route,-1,$vpr_log.".min_chan_width",$act_file);
    # Get the Minimum channel width
    my ($min_chan_width) = (&extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val}, $opt_ptr->{min_route_chan_width}, $parse_results));
    $min_chan_width = int($min_chan_width*$opt_ptr->{min_route_chan_width_val});
    if (0 != $min_chan_width%2) {
      $min_chan_width += 1;
    }
    # Remove previous route results
    if (-e $vpr_route) {
      system("rm $vpr_route");
    }
    # Keep increase min_chan_width until route success
    # Extract data from VPR stats
    #&run_std_vpr($abc_blif_out,$benchmark,$vpr_arch,$vpr_net,$vpr_place,$vpr_route,$min_chan_width,$vpr_log,$act_file);
    while (1) {
      &run_vpr_route($abc_blif_out,$benchmark,$vpr_arch,$vpr_net,$vpr_place,$vpr_route,$min_chan_width,$vpr_reroute_log,$act_file);
      # TODO: Only run the routing stage
      if (-e $vpr_route) {
        print "INFO: try route_chan_width($min_chan_width) success!\n";
        last; #Jump out
      } elsif ($max_route_width_retry < $min_chan_width) {
      # I set a threshold of 1000 as it is the limit of VPR 
        die "ERROR: Route Fail for $abc_blif_out with a min_chan_width of $min_chan_width!\n";
      } else {
        print "INFO: try route_chan_width($min_chan_width) failed! Retry with +2...\n";
        $min_chan_width += 2;
      }
    }
    if (1 == $parse_results) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val}, "off", $parse_results);
      &extract_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val});
      &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
    }
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    my ($fix_chan_width) = ($benchmarks_ptr->{$benchmark_file}->{fix_route_chan_width});
    # Remove previous route results
    if (-e $vpr_route) {
      system(rm $vpr_route);
    }
    # Keep increase min_chan_width until route success
    &run_std_vpr($abc_blif_out,$benchmark,$vpr_arch,$vpr_net,$vpr_place,$vpr_route,$fix_chan_width,$vpr_log,$act_file);
    while (1) {
      # TODO: Only run the routing stage
      if (-e $vpr_route) {
        print "INFO: try route_chan_width($fix_chan_width) success!\n";
        last; #Jump out
      } elsif ($max_route_width_retry < $fix_chan_width) {
      # I set a threshold of 1000 as it is the limit of VPR 
        die "ERROR: Route Fail for $abc_blif_out with a min_chan_width of $fix_chan_width!\n";
      } else {
        print "INFO: try route_chan_width($fix_chan_width) failed! Retry with +2...\n";
        $fix_chan_width += 2;
        &run_vpr_route($abc_blif_out,$benchmark,$vpr_arch,$vpr_net,$vpr_place,$vpr_route,$fix_chan_width,$vpr_reroute_log,$act_file);
      }
    }
    # Extract data from VPR stats
    if (1 == $parse_results) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val}, "off", $parse_results);
      &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
      if (-e $vpr_reroute_log) {
        &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
      }
    }
  } else {
    &run_std_vpr($abc_blif_out,$benchmark,$vpr_arch,$vpr_net,$vpr_place,$vpr_route,-1,$vpr_log,$act_file);
    if (!(-e $vpr_route)) {
      die "ERROR: Route Fail for $abc_blif_out!\n";
    }
    # Get the Minimum channel width
    my ($min_chan_width) = (&extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val},"on",$parse_results));
    if (1 == $parse_results) {
      &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
    }
  }

  # Extract data from VPR Power stats
  if (("on" eq $opt_ptr->{power})
     &&(1 == $parse_results)) {
    &extract_vpr_power_esti($tag,$abc_blif_out,$benchmark,$opt_ptr->{K_val});
  }

  return;
}


sub run_mig_mccl_flow($ $ $ $) {
  my ($tag,$benchmark_file,$vpr_arch, $parse_results) = @_;
  my ($benchmark, $rpt_dir,$prefix);
  my ($cirkit_bm,$cirkit_blif_out,$cirkit_log,$cirkit_blif_out_bak);

  $benchmark = $benchmark_file;
  $benchmark =~ s/\.blif$//g;
  # Run Standard flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $cirkit_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".blif";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $cirkit_blif_out = "$prefix"."cirkit.blif";
  $cirkit_blif_out_bak = "$prefix"."cirkit_bak.blif";
  $cirkit_log = "$prefix"."cirkit.log";

  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");

  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  &run_cirkit_mig_mccl_map($cirkit_bm,$cirkit_blif_out,$cirkit_log);
  if (!(-e $cirkit_blif_out)) {
    die "ERROR: Fail Cirkit for benchmark $cirkit_blif_out.\n";
  }

  #`perl pro_blif.pl -i $abc_blif_out_bak -o $abc_blif_out`;
  #if (!(-e $abc_blif_out)) {
  #  die "ERROR: Fail pro_blif.pl for benchmark $abc_blif_out.\n";
  #}

  &run_ace_in_flow($prefix, $cirkit_blif_out, $act_file, $ace_new_blif, $ace_log);

  &run_vpr_in_flow($tag, $benchmark, $benchmark_file, $cirkit_blif_out, $vpr_arch, $act_file, $vpr_net, $vpr_place, $vpr_route, $vpr_log, $vpr_reroute_log, $parse_results);

  return;
}

# Run Yosys-VPR flow
sub run_yosys_vpr_flow($ $ $ $ $)
{
  my ($tag,$benchmark_file,$vpr_arch,$flow_enhance, $parse_results) = @_;

  my ($benchmark, $rpt_dir, $prefix);
  my ($yosys_bm,$yosys_blif_out,$yosys_log,$yosys_blif_out_bak);

  my @tokens = split('/', $benchmark_file);
  $benchmark = $tokens[0];

  # Prepare for the output folder
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);

  # Adapt paths on architecture file
  `perl rewrite_path_in_file.pl -i $vpr_arch`;

  # Run Yosys flow
  $yosys_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark_file";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $yosys_blif_out = "$prefix"."yosys.blif";
  $yosys_log = "$prefix"."yosys.log";

  &run_yosys_fpgamap($benchmark, $yosys_bm, $yosys_blif_out, $yosys_log);

  # Files for ace
  my ($act_file,$ace_new_blif,$ace_log, $corrected_ace_blif) = ("$rpt_dir/$benchmark".".act","$rpt_dir/$benchmark"."ace.blif","$prefix"."ace.log","$rpt_dir/$benchmark".".blif");
  &run_ace_in_flow($prefix, $yosys_blif_out, $act_file, $ace_new_blif, $ace_log);

  &run_pro_blif_3arg($ace_new_blif, $corrected_ace_blif, $yosys_blif_out);

  # Files for VPR
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

# Need to add a regenation of the verilog from the optimized blif -> write verilog from blif + correct the name of the verilog for the testbench
  $verilog_benchmark = &run_rewrite_verilog($corrected_ace_blif, $rpt_dir, $benchmark, $benchmark, $yosys_log);

  &run_vpr_in_flow($tag, $benchmark, $benchmark_file, $corrected_ace_blif, $vpr_arch, $act_file, $vpr_net, $vpr_place, $vpr_route, $vpr_log, $vpr_reroute_log, $parse_results);

  if("on" eq $opt_ptr->{end_flow_with_test}) {
    &run_netlists_verification($benchmark);
  }

  return;
}

# Parse Yosys-VPR flow
sub parse_yosys_vpr_flow_results($ $ $ $)
{
  my ($tag,$benchmark_file,$vpr_arch,$flow_enhance) = @_;

  my ($benchmark, $rpt_dir, $prefix);
  my ($yosys_bm,$yosys_blif_out,$yosys_log,$yosys_blif_out_bak);

  my @tokens = split('/', $benchmark_file);
  $benchmark = $tokens[0];

  # Prepare for the output folder
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);

  # Run Yosys flow
  $yosys_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark_file";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $yosys_blif_out = "$rpt_dir/$benchmark".".blif";
  $yosys_log = "$prefix"."yosys.log";

  # Files for ace
  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");

  # Files for VPR
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val},"on",1);
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val});
    &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val},"off",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
    if (-e $vpr_reroute_log) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
      &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
    }
  } else {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val},"on",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
  }

  # Extract data from VPR Power stats
  if ("on" eq $opt_ptr->{power}) {
    &extract_vpr_power_esti($tag,$yosys_blif_out,$benchmark,$opt_ptr->{K_val});
  }

  # TODO: HOW TO DEAL WITH SPICE NETLISTS???
  # Output a file contain information of SPICE Netlists
  if ("on" eq $opt_ptr->{vpr_fpga_spice}) {
    &output_fpga_spice_task("$opt_ptr->{vpr_fpga_spice_val}"."_$tag.txt", $benchmark, $yosys_blif_out, $rpt_dir);
  }


  return;
}


sub run_standard_flow($ $ $ $ $)
{
  my ($tag,$benchmark_file,$vpr_arch,$flow_enhance, $parse_results) = @_;
  my ($benchmark, $rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log,$abc_blif_out_bak);
  my ($mpack_blif_out,$mpack_stats,$mpack_log);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  $benchmark = $benchmark_file;
  $benchmark =~ s/\.blif$//g;
  # Run Standard flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $abc_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".blif";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_blif_out_bak = "$prefix"."abc_bak.blif";
  $abc_log = "$prefix"."abc.log";


  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";


  if ("abc_black_box" eq $flow_enhance) {
    my ($pre_abc_blif) = ("$prefix"."pre_abc.blif");
    &run_pro_blif($abc_bm, $pre_abc_blif);
    &run_abc_bb_fpgamap($pre_abc_blif,$abc_blif_out_bak,$abc_log);
  } elsif ("classic" eq $flow_enhance) {
    &run_abc_fpgamap($abc_bm,$abc_blif_out_bak,$abc_log);
  }

  &run_pro_blif($abc_blif_out_bak, $abc_blif_out);

  &run_ace_in_flow($prefix, $abc_blif_out, $act_file, $ace_new_blif, $ace_log);

  &run_vpr_in_flow($tag, $benchmark, $benchmark_file, $abc_blif_out, $vpr_arch, $act_file, $vpr_net, $vpr_place, $vpr_route, $vpr_log, $vpr_reroute_log, $parse_results);

  return;
}

sub parse_standard_flow_results($ $ $ $)
{
  my ($tag,$benchmark_file,$vpr_arch,$flow_enhance) = @_;
  my ($rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log);
  my ($mpack_blif_out,$mpack_stats,$mpack_log);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  my ($benchmark) = ($benchmark_file);
  $benchmark =~ s/\.blif$//g;
  # Run Standard flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $abc_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".blif";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_log = "$prefix"."abc.log";

  if ("abc_black_box" eq $flow_enhance) {
    rename $abc_blif_out,"$abc_blif_out".".bak";
  } elsif ("classic" eq $flow_enhance) {
  }

  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val},"on",1);
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val});
    &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val},"off",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
    if (-e $vpr_reroute_log) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
      &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
    }
  } else {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val},"on",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
  }

  # Extract data from VPR Power stats
  if ("on" eq $opt_ptr->{power}) {
    &extract_vpr_power_esti($tag,$abc_blif_out,$benchmark,$opt_ptr->{K_val});
  }

  # TODO: HOW TO DEAL WITH SPICE NETLISTS???
  # Output a file contain information of SPICE Netlists
  if ("on" eq $opt_ptr->{vpr_fpga_spice}) {
    &output_fpga_spice_task("$opt_ptr->{vpr_fpga_spice_val}"."_standard.txt", $benchmark, $abc_blif_out, $rpt_dir);
  }

  return;
}

sub run_mpack2_flow($ $ $ $)
{
  my ($tag,$benchmark_file,$mpack2_arch,$parse_results) = @_;
  my ($rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log,$abc_blif_out_bak);
  my ($mpack2_blif_out,$mpack2_vpr_net,$mpack2_stats,$mpack2_log,$mpack2_vpr_arch);
  my ($vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log,$act_file);

  # Check necessary options
  if (!($opt_ptr->{N_val})) {
    die "ERROR: (mpack2_flow) -N should be specified!\n";
  }
  if (!($opt_ptr->{K_val})) {
    die "ERROR: (mpack2_flow) -K should be specified!\n";
  }

  my ($benchmark) = ($benchmark_file);
  $benchmark =~ s/\.blif$//g;
  # Run MPACK2-oriented flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $abc_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".blif";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_blif_out_bak = "$prefix"."abc_bak.blif";
  $abc_log = "$prefix"."abc.log";

  # RUN ABC
  #&run_abc_libmap($abc_bm,"$abc_blif_out\.bak",$abc_log);
  &run_abc_fpgamap($abc_bm,$abc_blif_out_bak,$abc_log);

  # Pre-process the blif netlist
  #`perl convert_blif.pl -i $abc_blif_out\.bak -o $abc_blif_out\.conv`;
  `perl pro_blif.pl -i $abc_blif_out_bak -o $abc_blif_out`;

  # RUN MPACK2
  $mpack2_blif_out = "$prefix"."mpack2.blif";
  $mpack2_vpr_net = "$prefix"."mpack2.net";
  $mpack2_stats = "$prefix"."mpack2.stats";
  $mpack2_log = "$prefix"."mpack2.log";
  $mpack2_vpr_arch = "$prefix"."mpack2_vpr_arch.xml";
  &run_mpack2($abc_blif_out,$mpack2_blif_out,$mpack2_arch,$mpack2_vpr_net,$mpack2_stats,$mpack2_vpr_arch,$mpack2_log);
  # Extract data from MPACK stats
  if (1 == $parse_results) {
    &extract_mpack2_stats($tag,$benchmark,$mpack2_stats);
  }

  # RUN VPR
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    &run_mpack2_vpr($mpack2_blif_out,$mpack2_vpr_arch,$mpack2_vpr_net,$vpr_place,$vpr_route,-1,$vpr_log.".min_chan_width");
    # Get the Minimum channel width
    my ($min_chan_width) = (&extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val}, $opt_ptr->{min_route_chan_width}, $parse_results));
    $min_chan_width = int($min_chan_width*$opt_ptr->{min_route_chan_width_val});
    if (0 != $min_chan_width%2) {
      $min_chan_width += 1;
    }

    # Remove previous route results
    system("rm $vpr_route");
    # Keep increase min_chan_width until route success
    # Extract data from VPR stats
    while (1) {
      &run_vpr_route($mpack2_blif_out,$benchmark,$mpack2_vpr_arch,$mpack2_vpr_net,$vpr_place,$vpr_route,$min_chan_width,$vpr_reroute_log,$act_file);
      if (-e $vpr_route) {
        print "INFO: try route_chan_width($min_chan_width) Success!\n";
        last; #Jump out
      } else {
        print "INFO: try route_chan_width($min_chan_width) failed! Retry with +2...\n";
        $min_chan_width += 2;
      }
    }
    # Extract data from VPR stats
    if (1 == $parse_results) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val}, "off", $parse_results);
      &extract_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val});
      &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
    }
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    my ($fix_chan_width) = ($benchmarks_ptr->{$benchmark_file}->{fix_route_chan_width});
    # Remove previous route results
    if (-e $vpr_route) {
      system("rm $vpr_route");
    }
    # Keep increase min_chan_width until route success
    # Extract data from VPR stats
    &run_mpack2_vpr($mpack2_blif_out,$mpack2_vpr_arch,$mpack2_vpr_net,$vpr_place,$vpr_route,$fix_chan_width,$vpr_log);
    while (1) {
      if (-e $vpr_route) {
        print "INFO: try route_chan_width($fix_chan_width) success!\n";
        last; #Jump out
      } else {
        print "INFO: try route_chan_width($fix_chan_width) failed! Retry with +2...\n";
        $fix_chan_width += 2;
        &run_vpr_route($mpack2_blif_out,$benchmark,$mpack2_vpr_arch,$mpack2_vpr_net,$vpr_place,$vpr_route,$fix_chan_width,$vpr_reroute_log,$act_file);
      }
    }
    if (1 == $parse_results) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val}, "off", $parse_results);
      &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
      if (-e $vpr_reroute_log) {
        &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val}, "off", $parse_results);
        &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
      }
    }
  } else {
    &run_mpack2_vpr($mpack2_blif_out,$mpack2_vpr_arch,$mpack2_vpr_net,$vpr_place,$vpr_route,-1,$vpr_log);
    if (!(-e $vpr_route)) {
      die "ERROR: Route Fail for $mpack2_blif_out!\n";
    }
    my ($min_chan_width) = (&extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val}, $parse_results));
    # Extract data from VPR stats
    if (1 == $parse_results) {
      &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
    }
  }
  return;
}

sub parse_mpack2_flow_results($ $ $)
{
  my ($tag,$benchmark_file,$mpack2_arch) = @_;
  my ($rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log);
  my ($mpack2_blif_out,$mpack2_vpr_net,$mpack2_stats,$mpack2_log,$mpack2_vpr_arch);
  my ($vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  # Check necessary options
  if (!($opt_ptr->{N_val})) {
    die "ERROR: (mpack2_flow) -N should be specified!\n";
  }
  if (!($opt_ptr->{K_val})) {
    die "ERROR: (mpack2_flow) -K should be specified!\n";
  }

  my ($benchmark) = ($benchmark_file);
  $benchmark =~ s/\.blif$//g;
  # Run MPACK2-oriented flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $abc_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".blif";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_log = "$prefix"."abc.log";

  # Pre-process the blif netlist

  # RUN MPACK2
  $mpack2_blif_out = "$prefix"."mpack2.blif";
  $mpack2_vpr_net = "$prefix"."mpack2.net";
  $mpack2_stats = "$prefix"."mpack2.stats";
  $mpack2_log = "$prefix"."mpack2.log";
  $mpack2_vpr_arch = "$prefix"."mpack2_vpr_arch.xml";
  # Extract data from MPACK stats
  &extract_mpack2_stats($tag,$benchmark,$mpack2_stats);

  # RUN VPR
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val},"on",1);
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val});
    &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
    if (-e $vpr_reroute_log) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
      &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
    } else {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val},"off",1);
    }
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
  } else {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val}, "on", 1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
  }

  return;
}

sub run_mpack1_flow($ $ $)
{
  my ($tag,$benchmark_file, $parse_results) = @_;
  my ($rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_log);
  my ($I_val,$M_val,$N_val) = ($opt_ptr->{I_val},$opt_ptr->{M_val},$opt_ptr->{N_val});
  my ($m2net_conf) = ($conf_ptr->{flow_conf}->{m2net_conf}->{val});
  my ($cell_size) = (2);

  if ($I_val) {
  } else {
    $I_val = int($cell_size*$M_val*($N_val+1)/2);
    print "INFO: I isn't defined. Auto-sized to 2*M*(N+1)/2 = $I_val\n";
  }

  my ($benchmark) = ($benchmark_file);
  $benchmark =~ s/\.blif$//g;
  # Run MPACK1-oriented flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $abc_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".blif";
  $prefix = "$rpt_dir/$benchmark\_"."M$M_val\_"."N$N_val\_";
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_log = "$prefix"."abc.log";

  &run_abc_libmap($abc_bm,"$abc_blif_out\.bak",$abc_log);
  `perl pro_blif.pl -i "$abc_blif_out\.bak" -o $abc_blif_out`;

  my ($mpack1_pack_blif_out) = ("$prefix"."_matrix.blif");
  my ($mpack1_vpr_blif_out) = ("$prefix"."_formatted.blif");
  my ($mpack1_rpt) = ("$prefix"."_mapped.net");
  my ($mpack1_log) = ("$prefix"."mpack1p5.log");
  &run_mpack1p5("$abc_blif_out","$prefix",$M_val,$cell_size,$mpack1_log);

  # Extract data from MPACK stats
  if (1 == $parse_results) {
    &extract_mpack1_stats($tag,$benchmark,$mpack1_log);
  }

  # Generate Architecture XML
  my ($aapack_arch) = ("$prefix"."aapack_arch.xml");
  my ($m2net_pack_arch_log) = ("$prefix"."m2net_pack_arch.log");
  &run_m2net_pack_arch($m2net_conf,$mpack1_rpt,$aapack_arch,$N_val,$I_val,$m2net_pack_arch_log);

  # Run AAPACK
  my ($aapack_log) = ("$prefix"."aapack.log");
  my ($aapack_net) = ("$prefix"."aapack.net");
  &run_aapack($mpack1_pack_blif_out,$aapack_arch,$aapack_net,$aapack_log);
  my @aapack_stats = ("MATRIX");
  if (1 == $parse_results) {
    &extract_aapack_stats($tag,$benchmark,$aapack_log,$M_val,\@aapack_stats);
  }

  $vpr_net = "$prefix"."mpack.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";

  # Run m2net.pl
  my ($vpr_arch) = ("$prefix"."vpr_arch.xml");
  my ($m2net_m2net_log) = ("$prefix"."m2net_m2net.log");
  &run_m2net_m2net($m2net_conf,$mpack1_rpt,$aapack_net,$vpr_net,$vpr_arch,$N_val,$I_val,$m2net_m2net_log);

  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace_new.blif","$prefix"."ace.log");
  # Turn on Power Estimation and Run ace
  if ("on" eq $opt_ptr->{power}) {
    &run_ace($mpack1_vpr_blif_out,$act_file,$ace_new_blif,$ace_log);
  }

  &run_mpack1_vpr($mpack1_vpr_blif_out,$vpr_arch,$vpr_net,$vpr_place,$vpr_route,$vpr_log,$act_file);
  if (!(-e $vpr_route)) {
    die "ERROR: Route Fail for $mpack1_vpr_blif_out!\n";
  }

  # Extract data from VPR stats
  if (1 == $parse_results) {
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$M_val);
  }

  if (("on" eq $opt_ptr->{power})
     &&(1 == $parse_results)) {
    &extract_vpr_power_esti($tag,$mpack1_vpr_blif_out,$benchmark,$M_val);
  }
}

sub parse_mpack1_flow_results($ $) {
  my ($tag,$benchmark_file) = @_;
  my ($rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_log);
  my ($I_val,$M_val,$N_val) = ($opt_ptr->{I_val},$opt_ptr->{M_val},$opt_ptr->{N_val});
  my ($m2net_conf) = ($conf_ptr->{flow_conf}->{m2net_conf}->{val});
  my ($cell_size) = (2);

  if ($I_val) {
  } else {
    $I_val = int($cell_size*$M_val*($N_val+1)/2);
    print "INFO: I isn't defined. Auto-sized to 2*M*(N+1)/2 = $I_val\n";
  }

  my ($benchmark) = ($benchmark_file);
  $benchmark =~ s/\.blif$//g;
  # Run MPACK1-oriented flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $abc_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".blif";
  $prefix = "$rpt_dir/$benchmark\_"."M$M_val\_"."N$N_val\_";
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_log = "$prefix"."abc.log";

  my ($mpack1_pack_blif_out) = ("$prefix"."_matrix.blif");
  my ($mpack1_vpr_blif_out) = ("$prefix"."_formatted.blif");
  my ($mpack1_rpt) = ("$prefix"."_mapped.net");
  my ($mpack1_log) = ("$prefix"."mpack1p5.log");

  # Extract data from MPACK stats
  &extract_mpack1_stats($tag,$benchmark,$mpack1_log);

  # Generate Architecture XML
  my ($aapack_arch) = ("$prefix"."aapack_arch.xml");
  my ($m2net_pack_arch_log) = ("$prefix"."m2net_pack_arch.log");

  # Run AAPACK
  my ($aapack_log) = ("$prefix"."aapack.log");
  my ($aapack_net) = ("$prefix"."aapack.net");
  my @aapack_stats = ("MATRIX");
  &extract_aapack_stats($tag,$benchmark,$aapack_log,$M_val,\@aapack_stats);

  $vpr_net = "$prefix"."mpack.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";

  # Run m2net.pl
  my ($vpr_arch) = ("$prefix"."vpr_arch.xml");
  my ($m2net_m2net_log) = ("$prefix"."m2net_m2net.log");
  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace_new.blif","$prefix"."ace.log");

  # Extract data from VPR stats
  &extract_vpr_stats($tag,$benchmark,$vpr_log,$M_val);

  if ("on" eq $opt_ptr->{power}) {
    &extract_vpr_power_esti($tag,$mpack1_vpr_blif_out,$benchmark,$M_val);
  }
}


sub run_vtr_flow($ $ $ $) {
  my ($tag,$benchmark_file,$vpr_arch,$parse_results) = @_;
  my ($rpt_dir,$prefix);
  my ($min_hard_adder_size, $mem_size, $odin2_verilog, $odin2_config, $odin2_log);
  my ($abc_bm,$abc_blif_out,$abc_log,$abc_blif_out_bak);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  # The input of VTR flow is verilog file
  my ($benchmark) = ($benchmark_file);
  $benchmark =~ s/\.v$//g;
  # Run Verilog To Routiing flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  # ODIN II output blif
  $odin2_verilog = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".v";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  # ODIN II config XML
  $odin2_config = "$prefix"."odin2_config.xml";
  $odin2_log = "$prefix"."odin2.log";
  # ODIN II output blif
  $abc_bm = "$prefix"."odin2.blif";
  # ABC II output blif
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_blif_out_bak = "$prefix"."abc_bak.blif";
  $abc_log = "$prefix"."abc.log";

  # Initialize min_hard_adder_size
  $min_hard_adder_size = 1; # Default value
  if ("on" eq $opt_ptr->{min_hard_adder_size}) {
    if (1 > $opt_ptr->{min_hard_adder_size_val}) {
      die "ERROR: Invalid min_hard_adder_size($opt_ptr->{min_hard_adder_size})!Should be no less than 1!";
    } else {
      $min_hard_adder_size = $opt_ptr->{min_hard_adder_size_val};
    }
  }
  # TODO: Initialize the mem_size by parsing the ARCH XML?
  if ("on" eq $opt_ptr->{mem_size}) {
    $mem_size = $opt_ptr->{mem_size_val};
  } else {
    die "ERROR: -mem_size is mandatory when vtr flow is chosen!\n";
  }
  # Auto-generate a configuration XML for ODIN2
  &gen_odin2_config_xml($odin2_config, $odin2_verilog, $abc_bm, $vpr_arch, $mem_size, $min_hard_adder_size);
  # RUN ODIN II
  &run_odin2($odin2_config, "off", $odin2_log);

  if (!(-e $abc_bm)) {
    die "ERROR: Fail ODIN II for benchmark $benchmark.\n";
  }

  # RUN ABC
  &run_abc_bb_fpgamap($abc_bm,$abc_blif_out_bak,$abc_log);

  &run_pro_blif($abc_blif_out_bak, $abc_blif_out);

  # Run ABC
  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");
  &run_ace_in_flow($prefix, $abc_blif_out,$act_file,$ace_new_blif,$ace_log);

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  # Run VPR
  &run_vpr_in_flow($tag, $benchmark, $benchmark_file, $abc_blif_out, $vpr_arch, $act_file, $vpr_net, $vpr_place, $vpr_route, $vpr_log, $vpr_reroute_log, $parse_results);

  return;
}

sub parse_vtr_flow_results($ $ $) {
  my ($tag,$benchmark,$vpr_arch) = @_;
  my ($min_hard_adder_size, $mem_size, $odin2_verilog, $odin2_config, $odin2_log);
  my ($rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log);
  my ($mpack_blif_out,$mpack_stats,$mpack_log);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  $benchmark =~ s/\.v$//g;
  # Run Standard flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  # ODIN II output blif
  $odin2_verilog = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".v";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  # ODIN II config XML
  $odin2_config = "$prefix"."odin2_config.xml";
  $odin2_log = "$prefix"."odin2.log";
  # ODIN II output blif
  $abc_bm = "$prefix"."odin2.blif";
  # ABC output blif
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_log = "$prefix"."abc.log";

  rename $abc_blif_out,"$abc_blif_out".".bak";

  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val},"on",1);
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log.".min_chan_width",$opt_ptr->{K_val});
    &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
    if (-e $vpr_reroute_log) {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val},"off",1);
      &extract_vpr_stats($tag,$benchmark,$vpr_reroute_log,$opt_ptr->{K_val});
    } else {
      &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val},"off",1);
    }
  } else {
    &extract_min_chan_width_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val}, "on", 1);
    &extract_vpr_stats($tag,$benchmark,$vpr_log,$opt_ptr->{K_val});
  }

  # Extract data from VPR Power stats
  if ("on" eq $opt_ptr->{power}) {
    &extract_vpr_power_esti($tag,$abc_blif_out,$benchmark,$opt_ptr->{K_val});
  }

  # TODO: HOW TO DEAL WITH SPICE NETLISTS???
  # Output a file contain information of SPICE Netlists
  if ("on" eq $opt_ptr->{vpr_fpga_spice}) {
    &output_fpga_spice_task("$opt_ptr->{vpr_fpga_spice_val}"."_vtr.txt", $benchmark, $abc_blif_out, $rpt_dir);
  }

  return;
}

# VTR_MCCL_flow:
# Differences from vtr_flow:
# 1. Need to turn off the carry-chain support for ODIN II
# 2. Use Carry-chain detection and Carry-chain LUTs pre-mapping in ABC scripts
sub run_vtr_mccl_flow($ $ $ $) {
  my ($tag,$benchmark_file,$vpr_arch,$parse_results) = @_;
  my ($rpt_dir,$prefix);
  my ($min_hard_adder_size, $mem_size, $odin2_verilog, $odin2_config, $odin2_log);
  my ($abc_bm,$abc_blif_out,$abc_log,$abc_blif_out_bak);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);
  my ($odin2_carry_chain_support) = ("on");

  # The input of VTR flow is verilog file
  my ($benchmark) = ($benchmark_file);
  $benchmark =~ s/\.v$//g;
  # Run Verilog To Routiing flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  # ODIN II output blif
  $odin2_verilog = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".v";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  # ODIN II config XML
  $odin2_config = "$prefix"."odin2_config.xml";
  $odin2_log = "$prefix"."odin2.log";
  # ODIN II output blif
  $abc_bm = "$prefix"."odin2.blif";
  # ABC II output blif
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_blif_out_bak = "$prefix"."abc_bak.blif";
  $abc_log = "$prefix"."abc.log";

  # Initialize min_hard_adder_size
  $min_hard_adder_size = 1; # Default value
  if ("on" eq $opt_ptr->{min_hard_adder_size}) {
    if (1 > $opt_ptr->{min_hard_adder_size_val}) {
      die "ERROR: Invalid min_hard_adder_size($opt_ptr->{min_hard_adder_size})!Should be no less than 1!";
    } else {
      $min_hard_adder_size = $opt_ptr->{min_hard_adder_size_val};
    }
  }
  # TODO: Initialize the mem_size by parsing the ARCH XML?
  if ("on" eq $opt_ptr->{mem_size}) {
    $mem_size = $opt_ptr->{mem_size_val};
  } else {
    die "ERROR: -mem_size is mandatory when vtr flow is chosen!\n";
  }
  # Auto-generate a configuration XML for ODIN2
  &gen_odin2_config_xml($odin2_config, $odin2_verilog, $abc_bm, $vpr_arch, $mem_size, $min_hard_adder_size);

  if ("on" eq $opt_ptr->{odin2_carry_chain_support}) {
    $odin2_carry_chain_support = ("on");
  }
  # RUN ODIN II
  &run_odin2($odin2_config, $odin2_carry_chain_support, $odin2_log);

  if (!(-e $abc_bm)) {
    die "ERROR: Fail ODIN II for benchmark $benchmark.\n";
  }

  # RUN ABC
  &run_abc_mccl_fpgamap($abc_bm,$abc_blif_out_bak,$abc_log);

  &run_pro_blif($abc_blif_out_bak, $abc_blif_out);

  # Run ACE
  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");
  &run_ace_in_flow($prefix,i $abc_blif_out,$act_file,$ace_new_blif,$ace_log);

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  # Run VPR
  &run_vpr_in_flow($tag, $benchmark, $benchmark_file, $abc_blif_out, $vpr_arch, $act_file, $vpr_net, $vpr_place, $vpr_route, $vpr_log, $vpr_reroute_log, $parse_results);

  return;
}

sub run_mccl_flow($ $ $ $ $)
{
  my ($tag,$benchmark_file,$vpr_arch,$flow_enhance, $parse_results) = @_;
  my ($benchmark, $rpt_dir,$prefix);
  my ($abc_bm,$abc_blif_out,$abc_log,$abc_blif_out_bak);
  my ($mpack_blif_out,$mpack_stats,$mpack_log);
  my ($vpr_net,$vpr_place,$vpr_route,$vpr_reroute_log,$vpr_log);

  $benchmark = $benchmark_file;
  $benchmark =~ s/\.v$//g; # We use verilog format in mccl
  # Run Standard flow
  $rpt_dir = "$conf_ptr->{dir_path}->{rpt_dir}->{val}"."/$benchmark/$tag";
  &generate_path($rpt_dir);
  $abc_bm = "$conf_ptr->{dir_path}->{benchmark_dir}->{val}"."/$benchmark".".v";
  $prefix = "$rpt_dir/$benchmark\_"."K$opt_ptr->{K_val}\_"."N$opt_ptr->{N_val}\_";
  $abc_blif_out = "$prefix"."abc.blif";
  $abc_blif_out_bak = "$prefix"."abc_bak.blif";
  $abc_log = "$prefix"."abc.log";

  # RUN ABC
  &run_abc_mccl_fpgamap($abc_bm,$abc_blif_out_bak,$abc_log);

  &run_pro_blif($abc_blif_out_bak, $abc_blif_out);

  # Run ACE
  my ($act_file,$ace_new_blif,$ace_log) = ("$prefix"."ace.act","$prefix"."ace.blif","$prefix"."ace.log");
  &run_ace_in_flow($prefix,i $abc_blif_out,$act_file,$ace_new_blif,$ace_log);

  $vpr_net = "$prefix"."vpr.net";
  $vpr_place = "$prefix"."vpr.place";
  $vpr_route = "$prefix"."vpr.route";
  $vpr_log = "$prefix"."vpr.log";
  $vpr_reroute_log = "$prefix"."vpr_reroute.log";

  # Run VPR
  &run_vpr_in_flow($tag, $benchmark, $benchmark_file, $abc_blif_out, $vpr_arch, $act_file, $vpr_net, $vpr_place, $vpr_route, $vpr_log, $vpr_reroute_log, $parse_results);

  return;
}

sub run_benchmark_selected_flow($ $ $)
{
  my ($flow_type,$benchmark, $parse_results) = @_;

  if ($flow_type eq "standard") {
    &run_standard_flow("standard",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val},"classic", $parse_results);
  } elsif ($flow_type eq "mpack2") {
    &run_mpack2_flow("mpack2",$benchmark,$conf_ptr->{flow_conf}->{mpack2_arch}->{val}, $parse_results);
  } elsif ($flow_type eq "mpack1") {
    &run_mpack1_flow("mpack1",$benchmark, $parse_results);
  } elsif ($flow_type eq "vtr_standard") {
    &run_standard_flow("vtr_standard",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val},"abc_black_box", $parse_results);
  } elsif ($flow_type eq "vtr") {
    &run_vtr_flow("vtr",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val}, $parse_results);
  } elsif ($flow_type eq "vtr_mccl") {
    &run_vtr_mccl_flow("vtr_mccl",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val}, $parse_results);
  } elsif ($flow_type eq "mccl") {
    &run_mccl_flow("mccl",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val}, $parse_results);
  } elsif ($flow_type eq "mig_mccl") {
    &run_mig_mccl_flow("mig_mccl",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val}, $parse_results);
  } elsif ($flow_type eq "yosys_vpr") {
    &run_yosys_vpr_flow("yosys_vpr",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val}, "classic", $parse_results);
  } else {
    die "ERROR: unsupported flow type ($flow_type) is chosen!\n";
  }

  return;
}

sub parse_benchmark_selected_flow($ $) {
  my ($flow_type,$benchmark) = @_;

  if ($flow_type eq "standard") {
    &parse_standard_flow_results("standard",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val},"classic");
  } elsif ($flow_type eq "mpack2") {
    &parse_mpack2_flow_results("mpack2",$benchmark,$conf_ptr->{flow_conf}->{mpack2_arch}->{val});
  } elsif ($flow_type eq "mpack1") {
    &parse_mpack1_flow_results("mpack1",$benchmark);
  } elsif ($flow_type eq "vtr_standard") {
    &parse_standard_flow_results("vtr_standard",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val},"abc_black_box");
  } elsif ($flow_type eq "vtr") {
    &parse_vtr_flow_results("vtr", $benchmark, $conf_ptr->{flow_conf}->{vpr_arch}->{val});
  } elsif ($flow_type eq "vtr_mccl") {
    &parse_vtr_flow_results("vtr_mccl", $benchmark, $conf_ptr->{flow_conf}->{vpr_arch}->{val});
  } elsif ($flow_type eq "mccl") {
    &parse_standard_flow_results("mccl", $benchmark, $conf_ptr->{flow_conf}->{vpr_arch}->{val}, "abc_black_box");
  } elsif ($flow_type eq "mig_mccl") {
    &parse_standard_flow_results("mig_mccl", $benchmark, $conf_ptr->{flow_conf}->{vpr_arch}->{val}, "abc_black_box");
  } elsif ($flow_type eq "yosys_vpr") {
    &parse_yosys_vpr_flow_results("yosys_vpr",$benchmark,$conf_ptr->{flow_conf}->{vpr_arch}->{val},"abc_black_box");
  } else {
    die "ERROR: unsupported flow type ($flow_type) is chosen!\n";
  }
}

# Run EDA flow
sub run_flows() {
  my @flows = split('\|',$conf_ptr->{flow_conf}->{flow_type}->{val});
  # Run Benchmark one by one
  foreach my $benchmark(@benchmark_names) {
    foreach my $flow_to_run(@flows) {
      if (("off" eq $selected_flows{$flow_to_run}->{flow_status})
         ||("done" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status})) {
        next;
      }
      print "FLOW TO RUN: $flow_to_run, Benchmark: $benchmark\n";
      &run_benchmark_selected_flow($flow_to_run,$benchmark, 0);
      # Mark finished benchmarks
      $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "done";
    }
  }
  &parse_flows_benchmarks_results();
}

# Run EDA flow with multi task support
sub multitask_run_flows() {
  my @flows = split('\|',$conf_ptr->{flow_conf}->{flow_type}->{val});
  # Run Benchmark one by one
  foreach my $benchmark(@benchmark_names) {
    foreach my $flow_to_run(@flows) {
      if (("off" eq $selected_flows{$flow_to_run}->{flow_status})
       ||("running" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status})
       ||("done" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status})) {
        next;
      }
      print "FLOW TO RUN: $flow_to_run, Benchmark: $benchmark\n";
      # Mutli thread push
      if ("on" eq $opt_ptr->{multi_task}) {
        my $pid = fork();
        if (defined $pid) {
          if ($pid) {
            $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "running";
            &run_benchmark_selected_flow($flow_to_run,$benchmark, 1);
            # Mark finished benchmarks
            $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "done";
          } else {
            exit;
          }
        } else {
          print "INFO: fail to create a thread for ";
          print "FLOW TO RUN: $flow_to_run, Benchmark: $benchmark\n";
          print "Relauch later...\n";
        }
      } else {
        &run_benchmark_selected_flow($flow_to_run,$benchmark, 1);
        # Mark finished benchmarks
        $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "done";
      }
    }
  }

  return;
}

# Run EDA flow with multi thread support
sub multithread_run_flows($) {
  my ($num_threads) = @_;
  my @flows = split('\|',$conf_ptr->{flow_conf}->{flow_type}->{val});
  # Evaluate include threads ok
  my ($can_use_threads) = (eval 'use threads; 1');
  if (!($can_use_threads)) {
    die "ERROR: cannot use threads package in Perl! Please check the installation of package...\n";
  }

  # Lauch threads up to the limited number of threads number
  if ($num_threads < 2) {
    $num_threads = 2;
  }
  my ($num_thread_running) = (0);

  # Iterate until all the tasks has been assigned, finished
  while (1 != &check_all_flows_all_benchmarks_done()) {
    foreach my $benchmark(@benchmark_names) {
      foreach my $flow_to_run(@flows) {
        # Bypass unselected flows or finished job
        if (("off" eq $selected_flows{$flow_to_run}->{flow_status})
           ||("done" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status})) {
          next;
        }
        # Check if the thread is still not start, running, or finished.
        my ($thr_id) = ($selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{thread_id});
        if ($thr_id) {
          # Check if there is any error
          if ($thr_id->error()) {
            die "Thread(ID:$thr_id) exit abnormally!\n";
          }
          # We have a thread id, check running or finished
          if ($thr_id->is_running()) {
            # Update status
            $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "running";
          }
          if ($thr_id->is_joinable()) {
            $num_thread_running--;
            $thr_id->join(); # Join the thread results
            # Update status
            $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "done";
            print "FLOW: $flow_to_run, Benchmark: $benchmark, Finished!\n";
            print "INFO: current running thread number = $num_thread_running.\n";
            &print_jobs_status();
          }
        } else {
          # Not start a thread for this task,
          if (($num_thread_running == $num_threads)
             ||($num_thread_running > $num_threads)) {
            next;
          }
          #if there are still threads available, we try to start one
          # Mutli thread push
          my $thr_new = threads->create(\&run_benchmark_selected_flow,$flow_to_run,$benchmark, 0);
          # We have a valid thread...
          if ($thr_new) {
            print "INFO: a new thread is lauched!\n";
            print "FLOW RUNNING: $flow_to_run, Benchmark: $benchmark\n";
            # Check if it is running...
            if ($thr_new->is_running()) {
              $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "running";
              $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{thread_id} = $thr_new;
              $num_thread_running++;
              print "INFO: current running thread number = $num_thread_running.\n";
              &print_jobs_status();
            }
            # Check if it is detached...
            if ($thr_new->is_joinable()) {
              # Mark finished benchmarks
              $num_thread_running--;
              $thr_new->join(); # Join the thread results
              $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status} = "done";
              print "FLOW: $flow_to_run, Benchmark: $benchmark, Finished!\n";
              print "INFO: current running thread number = $num_thread_running.\n";
              &print_jobs_status();
            }
          } else {
            # Fail to create a new thread, wait...
            print "INFO: Fail to alloc a new thread, wait...!";
          }
        }
      }
    }
  }
  &print_jobs_status();

  &parse_flows_benchmarks_results();

  return;
}

sub parse_flows_benchmarks_results() {
  # Parse all the results
  foreach my $benchmark(@benchmark_names) {
    foreach my $flow_to_run(@supported_flows) {
      # Bypass unselected flows or finished job
      if (("on" eq $selected_flows{$flow_to_run}->{flow_status})
         &&("done" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status})) {
        &parse_benchmark_selected_flow($flow_to_run, $benchmark);
      }
    }
  }

  return;
}

sub print_jobs_status() {
  my ($num_jobs_running, $num_jobs_to_run, $num_jobs_finish, $num_jobs) = (0, 0, 0, 0);

  foreach my $benchmark(@benchmark_names) {
    foreach my $flow_to_run(@supported_flows) {
      if ("on" eq $selected_flows{$flow_to_run}->{flow_status}) {
        # Count the number of jobs
        $num_jobs++;
        # Count to do jobs
        if ("off" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status}) {
          $num_jobs_to_run++;
          next;
        }
        # Count running jobs
        if ("running" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status}) {
          $num_jobs_running++;
          next;
        }
        # Count finished jobs
        if ("done" eq $selected_flows{$flow_to_run}->{benchmarks}->{$benchmark}->{status}) {
          $num_jobs_finish++;
          next;
        }
      }
    }
  }
  if ($num_jobs == ($num_jobs_running + $num_jobs_finish + $num_jobs_to_run)) {
    print "Jobs Progress: (Finish rate = ".sprintf("%.2f",100*$num_jobs_finish/$num_jobs) ."%)\n";
    print "Total No. of Jobs: $num_jobs.\n";
    print "No. of Running Jobs: $num_jobs_running.\n";
    print "No. of Finished Jobs: $num_jobs_finish.\n";
    print "No. of To Run Jobs: $num_jobs_to_run.\n";
  } else {
    print "Internal problem: num_jobs($num_jobs) != num_jobs_running($num_jobs_running)\n";
    print "                                        +num_jobs_finish($num_jobs_finish)\n";
      die "                                        +num_jobs_to_run($num_jobs_to_run)\n";
  }
  return;
}

sub check_all_flows_all_benchmarks_done() {
  my ($all_done) = (1);
  foreach my $flow_to_run(@supported_flows) {
    if ("off" eq $selected_flows{$flow_to_run}->{flow_status}) {
      next;
    }
    if (1 != &check_flow_all_benchmarks_done($flow_to_run)) {
      $all_done = 0;
      last;
    }
  }
  return $all_done;
}

sub check_flow_all_benchmarks_done($) {
  my ($flow_name) = @_;
  my ($all_done) = (0);
  # If this flow has not been chosen, return 0
  if ("off" eq $selected_flows{$flow_name}->{flow_status}) {
    return $all_done;
  } elsif ("on" eq $selected_flows{$flow_name}->{flow_status}) {
    $all_done = 1;
  }
  # Check if every benchmark has finished in this flow.
  foreach my $bm(@benchmark_names) {
    if ("done" ne $selected_flows{$flow_name}->{benchmarks}->{$bm}->{status}) {
      $all_done = 0;
      last;
    }
  }

  return $all_done;
}

sub gen_csv_rpt_vtr_flow($ $)
{
  my ($tag,$CSVFH) = @_;
  my ($tmp,$ikw,$tmpkw);
  my @keywords;
  my ($K_val,$N_val) = ($opt_ptr->{K_val},$opt_ptr->{N_val});

  # adapt to matlab format if the option is enabled 
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    # Print the data name 
    print $CSVFH "$opt_ptr->{matlab_rpt_val} = [\n";
    # We will set the stats line to be commented 
    print $CSVFH "%";
  }

  # Print out Standard Stats First
  print $CSVFH "$tag";
  print $CSVFH ",LUTs";
  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    print $CSVFH ",min_route_chan_width";
    print $CSVFH ",fix_route_chan_width";
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    print $CSVFH ",fix_route_chan_width";
  } else {
    print $CSVFH ",min_route_chan_width";
  }
  @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
  #foreach $tmpkw(@keywords) {
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  if ("on" eq $opt_ptr->{power}) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
    #foreach $tmpkw(@keywords) {
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      print $CSVFH ",$keywords[$ikw]";
    }
    print $CSVFH ",Total Power,Total Dynamic Power,Total Leakage Power";
  }
  print $CSVFH "\n";
  # Check log/stats one by one
  foreach $tmp(@benchmark_names) {
    $tmp =~ s/\.v$//g;     
    print $CSVFH "$tmp";
    print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{LUTs}";
    if ("on" eq $opt_ptr->{min_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } else {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
    }
    #foreach $tmpkw(@keywords) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{$keywords[$ikw]}";
    }
    if ("on" eq $opt_ptr->{power}) {
      @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
      for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
        $tmpkw = $keywords[$ikw];
        $tmpkw =~ s/\s//g;
        print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{$keywords[$ikw]}";
      }
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{total}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{dynamic}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{leakage}";
    }
    # For matlab script, we end with a semicolumn to be compatiable to matlab
    if ("on" eq $opt_ptr->{matlab_rpt}) {
      print $CSVFH ";\n";
    } else {
      print $CSVFH "\n";
    }
  }

  # For matlab script, we end with ];
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    print $CSVFH "];\n";
  }
}

sub gen_csv_rpt_yosys_vpr_flow($ $)
{
  my ($tag,$CSVFH) = @_;
  my ($tmp,$ikw,$tmpkw);
  my @keywords;
  my ($K_val,$N_val) = ($opt_ptr->{K_val},$opt_ptr->{N_val});
 
  # adapt to matlab format if the option is enabled 
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    # Print the data name 
    print $CSVFH "$opt_ptr->{matlab_rpt_val} = [\n";
    # We will set the stats line to be commented 
    print $CSVFH "%";
  }

  # Print out Standard Stats First
  print $CSVFH "$tag";
  print $CSVFH ",LUTs";
  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    print $CSVFH ",min_route_chan_width";
    print $CSVFH ",fix_route_chan_width";
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    print $CSVFH ",fix_route_chan_width";
  } else {
    print $CSVFH ",min_route_chan_width";
  }
  @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
  #foreach $tmpkw(@keywords) {
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  if ("on" eq $opt_ptr->{power}) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
    #foreach $tmpkw(@keywords) {
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      print $CSVFH ",$keywords[$ikw]";
    }
    print $CSVFH ",Total Power,Total Dynamic Power,Total Leakage Power";
  }
  print $CSVFH "\n";
  # Check log/stats one by one
  foreach $tmp(@benchmark_names) {
    my @tokens = split('/', $tmp);
    $tmp = $tokens[0];

    # For matlab script, we use {} for string 
    if ("on" eq $opt_ptr->{matlab_rpt}) {
      print $CSVFH "{'$tmp'}"; 
    } else {
      print $CSVFH "$tmp";
    }

    print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{LUTs}";
    if ("on" eq $opt_ptr->{min_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } else {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
    }
    #foreach $tmpkw(@keywords) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{$keywords[$ikw]}";
    }
    if ("on" eq $opt_ptr->{power}) {
      @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
      for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
        $tmpkw = $keywords[$ikw];
        $tmpkw =~ s/\s//g;
        print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{$keywords[$ikw]}";
      }
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{total}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{dynamic}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{leakage}";
    }
    # For matlab script, we end with a semicolumn to be compatiable to matlab
    if ("on" eq $opt_ptr->{matlab_rpt}) {
      print $CSVFH ";\n";
    } else {
      print $CSVFH "\n";
    }
  }

  # For matlab script, we end with ];
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    print $CSVFH "];\n";
  }
}

sub gen_csv_rpt_standard_flow($ $)
{
  my ($tag,$CSVFH) = @_;
  my ($tmp,$ikw,$tmpkw);
  my @keywords;
  my ($K_val,$N_val) = ($opt_ptr->{K_val},$opt_ptr->{N_val});
 
  # adapt to matlab format if the option is enabled 
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    # Print the data name 
    print $CSVFH "$opt_ptr->{matlab_rpt_val} = [\n";
    # We will set the stats line to be commented 
    print $CSVFH "%";
  }

  # Print out Standard Stats First
  print $CSVFH "$tag";
  print $CSVFH ",LUTs";
  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    print $CSVFH ",min_route_chan_width";
    print $CSVFH ",fix_route_chan_width";
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    print $CSVFH ",fix_route_chan_width";
  } else {
    print $CSVFH ",min_route_chan_width";
  }
  @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
  #foreach $tmpkw(@keywords) {
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  if ("on" eq $opt_ptr->{power}) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
    #foreach $tmpkw(@keywords) {
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      print $CSVFH ",$keywords[$ikw]";
    }
    print $CSVFH ",Total Power,Total Dynamic Power,Total Leakage Power";
  }
  print $CSVFH "\n";
  # Check log/stats one by one
  foreach $tmp(@benchmark_names) {
    $tmp =~ s/\.blif$//g;     
    print $CSVFH "$tmp";
    print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{LUTs}";
    if ("on" eq $opt_ptr->{min_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } else {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
    }
    #foreach $tmpkw(@keywords) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;  
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{$keywords[$ikw]}";
    }
    if ("on" eq $opt_ptr->{power}) {
      @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
      for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
        $tmpkw = $keywords[$ikw];
        $tmpkw =~ s/\s//g;
        print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{$keywords[$ikw]}";
      }
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{total}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{dynamic}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{leakage}";
    }

    # For matlab script, we end with a semicolumn to be compatiable to matlab
    if ("on" eq $opt_ptr->{matlab_rpt}) {
      print $CSVFH ";\n";
    } else {
      print $CSVFH "\n";
    }
  }

  # For matlab script, we end with ];
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    print $CSVFH "];\n";
  }
}

sub gen_csv_rpt_mpack2_flow($ $)
{
  my ($tag,$CSVFH) = @_;
  my ($tmp,$ikw,$tmpkw);
  my @keywords;
  my ($K_val,$N_val) = ($opt_ptr->{K_val},$opt_ptr->{N_val});

  # adapt to matlab format if the option is enabled 
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    # Print the data name 
    print $CSVFH "$opt_ptr->{matlab_rpt_val} = [\n";
    # We will set the stats line to be commented 
    print $CSVFH "%";
  }

  # Print out Mpack stats Second
  print $CSVFH "$tag";
  if ("on" eq $opt_ptr->{min_route_chan_width}) {
    print $CSVFH ",min_route_chan_width";
    print $CSVFH ",fix_route_chan_width";
  } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
    print $CSVFH ",fix_route_chan_width";
  } else {
    print $CSVFH ",min_route_chan_width";
  }

  @keywords = split /\|/,$conf_ptr->{csv_tags}->{mpack2_tags}->{val};
  #foreach $tmpkw(@keywords) {
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
  #foreach $tmpkw(@keywords) {
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  if ("on" eq $opt_ptr->{power}) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
    #foreach $tmpkw(@keywords) {
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      print $CSVFH ",$keywords[$ikw]";
    }
    print $CSVFH ",Total Power,Total Dynamic Power,Total Leakage Power";
  }
  print $CSVFH "\n";
  # Check log/stats one by one
  foreach $tmp(@benchmark_names) {
    $tmp =~ s/\.blif$//g;     
    print $CSVFH "$tmp";
    if ("on" eq $opt_ptr->{min_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } elsif ("on" eq $opt_ptr->{fix_route_chan_width}) {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{fix_route_chan_width}";
    } else {
      print $CSVFH ",$rpt_h{$tag}->{$tmp}->{$N_val}->{$K_val}->{min_route_chan_width}";
    }
     #foreach $tmpkw(@keywords) {
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{mpack2_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{$keywords[$ikw]}";
    }
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{$keywords[$ikw]}";
    }
    if ("on" eq $opt_ptr->{power}) {
      @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
      for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
        $tmpkw = $keywords[$ikw];
        $tmpkw =~ s/\s//g;
        print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{$keywords[$ikw]}";
      }
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{total}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{dynamic}";
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$K_val}->{power}->{leakage}";
    }
    # For matlab script, we end with a semicolumn to be compatiable to matlab
    if ("on" eq $opt_ptr->{matlab_rpt}) {
      print $CSVFH ";\n";
    } else {
      print $CSVFH "\n";
    }
  }

  # For matlab script, we end with ];
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    print $CSVFH "];\n";
  }
}

sub gen_csv_rpt_mpack1_flow($ $)
{
  my ($tag,$CSVFH) = @_;
  my ($tmp,$ikw,$tmpkw);
  my @keywords;
  my ($N_val,$M_val) = ($opt_ptr->{N_val},$opt_ptr->{M_val});

  # adapt to matlab format if the option is enabled 
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    # Print the data name 
    print $CSVFH "$opt_ptr->{matlab_rpt_val} = [\n";
    # We will set the stats line to be commented 
    print $CSVFH "%";
  }

  # Print out Mpack stats Second
  print $CSVFH "$tag";
  print $CSVFH ",MATRIX";
  @keywords = split /\|/,$conf_ptr->{csv_tags}->{mpack_tags}->{val};
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
  #foreach $tmpkw(@keywords) {
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  # Print Power Tags
  @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
  #foreach $tmpkw(@keywords) {
  for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
    print $CSVFH ",$keywords[$ikw]";
  }
  print $CSVFH ",Total Power,Total Dynamic Power, Total Leakage Power";
  print $CSVFH "\n";
  # Check log/stats one by one
  foreach $tmp(@benchmark_names) {
    $tmp =~ s/\.blif$//g;     
    print $CSVFH "$tmp";
    #foreach $tmpkw(@keywords) {
    print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$M_val}->{MATRIX}";
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{mpack_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$M_val}->{$keywords[$ikw]}";
    }
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$M_val}->{$keywords[$ikw]}";
    }
    # Print Power Results
    @keywords = split /\|/,$conf_ptr->{csv_tags}->{vpr_power_tags}->{val};
    for($ikw=0; $ikw < ($#keywords+1); $ikw++) {
      $tmpkw = $keywords[$ikw];
      $tmpkw =~ s/\s//g;
      print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$M_val}->{power}->{$keywords[$ikw]}";
    }
    print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$M_val}->{power}->{total}";
    print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$M_val}->{power}->{dynamic}";
    print $CSVFH ",$rpt_ptr->{$tag}->{$tmp}->{$N_val}->{$M_val}->{power}->{leakage}";
    # For matlab script, we end with a semicolumn to be compatiable to matlab
    if ("on" eq $opt_ptr->{matlab_rpt}) {
      print $CSVFH ";\n";
    } else {
      print $CSVFH "\n";
    }
  }

  # For matlab script, we end with ];
  if ("on" eq $opt_ptr->{matlab_rpt}) {
    print $CSVFH "];\n";
  }
}

sub init_selected_flows() {
  # For each flow type, mark the status to off
  foreach my $flow_type(@supported_flows) {
    $selected_flows{$flow_type}->{flow_status} = "off";
    # For each benchmark, init the status to "off"
    foreach my $benchmark(@benchmark_names) {
      $selected_flows{$flow_type}->{benchmarks}->{$benchmark}->{status} = "off";
      $selected_flows{$flow_type}->{benchmarks}->{$benchmark}->{thread_id} = undef;
    }
  }
}

sub mark_selected_flows()
{
  # Mark what flows are selected
  my @flows = split('\|',$conf_ptr->{flow_conf}->{flow_type}->{val});
  foreach my $flow_type(@flows) {
    if (exists $selected_flows{$flow_type}->{flow_status}) {
      $selected_flows{$flow_type}->{flow_status} = "on";
      print "INFO: FLOW TYPE: $flow_type is turned $selected_flows{$flow_type}->{flow_status}\n";
      # Initial FPGA SPICE TASK FILE
      if ("on" eq $opt_ptr->{vpr_fpga_spice}) {
        &init_fpga_spice_task($opt_ptr->{vpr_fpga_spice_val}."_$flow_type.txt");
      }
    } else {
      die "ERROR: flow_type: $flow_type is not supported!\n";
    }
  }
}

sub mark_flows_benchmarks() {
  foreach my $flow_type(@supported_flows) {
    if ("on" eq $selected_flows{$flow_type}->{flow_status}) {
      # For each benchmark, init the status to "off"
      foreach my $benchmark(@benchmark_names) {
        $selected_flows{$flow_type}->{benchmarks}->{$benchmark}->{status} = "done";
      }
    }
  }
}

sub gen_csv_rpt($)
{
  my ($csv_file) = @_;

  my ($csv_dir_path, $csv_filename) = &split_prog_path($csv_file);
  &generate_path($csv_dir_path);

  # Open a filehandle
  my ($CSVFH) = (FileHandle->new);
  if ($CSVFH->open("> $csv_file")) {
    print "INFO: writing CSV report ($csv_file) ...\n";
  } else {
    die "ERROR: fail to create CSV report ($csv_file) ...\n";
  }

  foreach my $flow_type(@supported_flows) {
    if ($selected_flows{$flow_type}->{flow_status} eq "on") {
      # Print the report only all the benchmarks in this flow finished
      if ($flow_type eq "standard") {
        if (1 == &check_flow_all_benchmarks_done("standard")) {
          print "INFO: writing standard flow results ...\n";
          &gen_csv_rpt_standard_flow("standard",$CSVFH);
        }
      } elsif ($flow_type eq "mpack2") {
        if (1 == &check_flow_all_benchmarks_done("mpack2")) {
          print "INFO: writing mpack2 flow results ...\n";
          &gen_csv_rpt_mpack2_flow("mpack2",$CSVFH);
        }
      } elsif ($flow_type eq "mpack1") {
        if (1 == &check_flow_all_benchmarks_done("mpack1")) {
          print "INFO: writing mpack1 flow results ...\n";
          &gen_csv_rpt_mpack1_flow("mpack1",$CSVFH);
        }
      } elsif ($flow_type eq "vtr_standard") {
        if (1 == &check_flow_all_benchmarks_done("vtr")) {
          print "INFO: writing vtr flow results ...\n";
          &gen_csv_rpt_standard_flow("vtr_standard",$CSVFH);
        }
      } elsif ($flow_type eq "vtr") {
        if (1 == &check_flow_all_benchmarks_done("vtr")) {
          print "INFO: writing vtr flow results ...\n";
          &gen_csv_rpt_vtr_flow("vtr",$CSVFH);
        }
      } elsif ($flow_type eq "vtr_mccl") {
        if (1 == &check_flow_all_benchmarks_done("vtr_mccl")) {
          print "INFO: writing vtr_mccl flow results ...\n";
          &gen_csv_rpt_standard_flow("vtr_mccl",$CSVFH);
        }
      } elsif ($flow_type eq "mccl") {
        if (1 == &check_flow_all_benchmarks_done("mccl")) {
          print "INFO: writing mccl flow results ...\n";
          &gen_csv_rpt_standard_flow("mccl",$CSVFH);
        }
      } elsif ($flow_type eq "mig_mccl") {
        if (1 == &check_flow_all_benchmarks_done("mig_mccl")) {
          print "INFO: writing mig_mccl flow results ...\n";
          &gen_csv_rpt_standard_flow("mig_mccl",$CSVFH);
        }
      } elsif ($flow_type eq "yosys_vpr") {
        if (1 == &check_flow_all_benchmarks_done("yosys_vpr")) {
          print "INFO: writing yosys_vpr flow results ...\n";
          &gen_csv_rpt_yosys_vpr_flow("yosys_vpr",$CSVFH);
        }
      } else {
        die "ERROR: flow_type: $flow_type is not supported!\n";
      }
    }
  }

  close($CSVFH);
}

sub remove_designs()
{
  if ("on" eq $opt_ptr->{remove_designs}) {
    system("rm -rf $conf_ptr->{dir_path}->{rpt_dir}->{val}");
  }
}

sub plan_run_flows() {

  if ("on" eq $opt_ptr->{multi_task}) {
    &multitask_run_flows();
  } elsif (("on" eq $opt_ptr->{multi_thread})
          &&($opt_ptr->{multi_thread_val} > 1)
          &&(0 < $#benchmark_names)) {
    &multithread_run_flows($opt_ptr->{multi_thread_val});
  } else {
    if ("on" eq $opt_ptr->{multi_thread}) {
      print "INFO: multi_thread is selected but only 1 processor can be used or 1 benchmark to run...\n";
      print "INFO: switch to single thread mode.\n";
    }
    &run_flows();
  }
}

# Main Program
sub main()
{
  &opts_read();
  &read_conf();
  &read_benchmarks();
  &init_selected_flows();
  &mark_selected_flows();
  &check_opts();
  if ("on" eq $opt_ptr->{parse_results_only}) {
    &mark_flows_benchmarks();
    &parse_flows_benchmarks_results();
  } else {
    &remove_designs();
    &plan_run_flows();
  }
  &gen_csv_rpt($opt_ptr->{rpt_val});
}

&main();
exit(0);
