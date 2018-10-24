#!usr/bin/perl -w
# Perl Script to convert MPACK1 netlist for VPR and generate architecture file
# use the strict mode
use strict;
# Use the Shell enviornment
#use Shell;
# Use the time
use Time::gmtime;
# Use switch module
use Switch;
use File::Path;
use Cwd;

# Date
my $mydate = gmctime(); 
# Current Path
my $cwd = getcwd();

# Global Variants
# input Option Hash
my %opt_h; 
my $opt_ptr = \%opt_h;
# configurate file hash
my %conf_h;
my $conf_ptr = \%conf_h;
# reports has
my %rpt_h;
my $rpt_ptr = \%rpt_h;

# Matrix informations
my %mclusters;
my ($mclusters_ptr) = (\%mclusters);

# Configuration file keywords list
# Category for conf file.
# main category : 1st class
my @mctgy;
# sub category : 2nd class
my @sctgy;
# Initialize these categories
@mctgy = ("arch_model",
          "arch_device",
          "arch_complexblocks",
         );
# refer to the keywords of arch_model
@{$sctgy[0]} = ("matrix_model_name",
                "matrix_inport_name",
                "matrix_outport_name",
                "cell_model_name",
                "cell_inport_name",
                "cell_outport_name",
               );
# refer to the keywords of arch_device
# Support uni-directional routing architecture and
# single type segment only
@{$sctgy[1]} = ("R_minW_nmos",
                "R_minW_pmos",
                "ipin_mux_trans_size",
                "C_ipin_cblock",
                "T_ipin_cblock",
                "grid_logic_tile_area",
                "mux_R",
                "mux_Cin",
                "mux_Cout",
                "mux_Tdel",
                "mux_trans_size",
                "mux_buf_size",
                "segment_length",
                "segment_Rmetal",
                "segment_Cmetal",
                "local_interconnect_C_wire",
                "clock_buffer_size",
                "clock_C_wire",
                );
# refer to the keywords of arch_complexblocks
@{$sctgy[2]} = (#"io_capacity", should be automatically optimized as 2*sqrt(N)
                "CLB_logic_equivalent",
                #"matrix_name",
                #"matrix_cell_name",
                "matrix_delay",
                "cell_delay",
                "dff_tsetup",
                "dff_tclk2q",
                "mux2to1_delay", # Delay of a 2:1 multiplexer, script can estimate N:1 multiplexer
                "cell_dynamic_power",
                "cell_static_power",
               );

my ($SiNW_area_ratio) = (1.5);
# ----------Subrountines------------#

# Print TABs
sub print_tabs($ $)
{
  my ($num_tab,$FILE) = @_;
  my ($my_tab) = ("    ");
  
  for (my $i = 0; $i < $num_tab; $i++) {
    print $FILE "$my_tab";
  }
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

# Print the usage
sub print_usage()
{
  print "Usage:\n";
  print "  perl m2net.pl [-options <value>]\n";
  print "    Mandatory options: \n";
  print "      -conf : specify the basic configuration files for m2net\n";
  print "      -mpack1_rpt : MPACK1 report file\n";
  print "      -mode <pack_arch|m2net> : select mode\n";
  print "            1. pack_arch : only output XML architecture file for AApack\n";
  print "            2. m2net : output XML architecture file for VPR, convert *.net file for VPR\n";
  print "      -N : Number of MCluster inside a CLB\n";
  print "      -I : Number of input of a CLB\n";
  #print "      -rpt : m2net running log\n";
  print "    Mandatory options for -mode pack_arch:\n";
  print "      -arch_file_pack : filename of output XML format architecture file for AAPack\n";
  print "    Mandatory options for -mode m2net:\n";
  print "      -net_file_in : filename of input *.net file from AAPack\n";
  print "      -net_file_out : filename of output *.net file for VPR\n";
  print "      -arch_file_vpr : filename of output XML format architecture file for VPR\n";
  print "    Other Options:\n";
  print "      -power : add power estimation information to VPR ARCH XML\n";
  print "      -debug : debug mode\n";
  print "      -help : print usage\n";
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
  return 1;
}

# Read options
sub opts_read()
{
  # if no arguments detected, print the usage.
  if (-1 == $#ARGV)
  {
    print "Error : No input arguments!\n";
    print "Try: -help for usage.\n";
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

  # Read Opt into Hash(opt_ptr) : "opt_name","with_val","mandatory"

  # Check mode first
  &read_opt_into_hash("mode","on","on");

  # Check mandatory options
  &read_opt_into_hash("conf","on","on"); # Check -conf
  &read_opt_into_hash("mpack1_rpt","on","on"); # Check -mpack1_rpt
  &read_opt_into_hash("N","on","on");    # Check -N
  &read_opt_into_hash("I","on","on");    # Check -I
  #&read_opt_into_hash("rpt","on","on");  # Check -rpt
  
  # Check mandatory options by mode selected
  if ("pack_arch" eq $opt_ptr->{"mode_val"}) {
    &read_opt_into_hash("arch_file_pack","on","on"); # Check -arch_file_mpack
  }
  elsif ("m2net" eq $opt_ptr->{"mode_val"}) {
    &read_opt_into_hash("arch_file_vpr","on","on"); # Check -arch_file_vpr 
    &read_opt_into_hash("net_file_in","on","on");   # Check -net_file_in
    &read_opt_into_hash("net_file_out","on","on");  # Check -net_file_out
  }
  else {
    print "Error: unknown mode!\n";
    print "Help desk:\n";
    &print_usage();
  }
  &read_opt_into_hash("power","off","off");    # Check -power

  &opts_echo(); 

  return 1;
}
  
# List the options
sub opts_echo()
{
  print "Echo your options:\n"; 
 
  while(my ($key,$value) = each(%opt_h))
  {print "$key : $value\n";}

  return 1;
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
  return 1;
}

# Read the configuration file
sub read_conf()
{
  # Read in these key words
  my ($line,$post_line);
  my @equation;
  my $cur = "unknown";
  open (CONF, "< $opt_ptr->{conf_val}") or die "Fail to open $opt_ptr->{conf}!\n";
  print "Reading $opt_ptr->{conf_val}...";
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
  print "complete!\n";
  print "Checking these keywords...";
  &check_keywords_conf(); 
  print "Successfully\n";
  close(CONF);
  return 1;
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

# Read MPACK1 Report
# Determine matrix width, matrix height
# Determine matrix internal connections
# Store pack information for each cell
sub read_mpack1_rpt()
{
  # Read in these key words
  my ($line,$post_line,$line_no);
  my ($layer_line_cnt,$mcluster_line_cnt,$mcluster_cnt);
  my ($checking_layer,$checking_mclusters,$expected_layer_line_no,$expected_mcluster_line_no) = (0,0,-1,-1);
  my ($x,$y);
  my @split_line;
  # Define keywords
  my ($matrix_width,$matrix_depth,$cell_size,$layer,$layer_matrix,$mcluster,$mcluster_cell,$unconn,$conn,$open_net,$end,$mcluster_num) = ("matrix_width","matrix_depth","cell_size","layer_detailed","X","MCluster","cell","unconn","conn","open","end","mclusters_number");

  open (MRPT, "< $opt_ptr->{mpack1_rpt_val}") or die "Fail to open mpack1_rpt: $opt_ptr->{mpack1_rpt_val}!\n";
  print "Reading $opt_ptr->{mpack1_rpt_val}...";

  $line_no = 0;
  $mcluster_cnt = 0;

  while(defined($line = <MRPT>))
  {
    chomp $line;
    $post_line = &read_line($line,"#"); 
    $line_no = $line_no + 1;
    if (defined($post_line))
    {
      # Remove all spaces...
      $post_line =~ s/\s//g;
      # This case should not happen!
      if ((1 == $checking_layer)&&(1 == $checking_mclusters)) {
        die "Error: checking_layer and checking_mclusters both turn on!\n";
      }
      # TODO List:
      if ((0 == $checking_layer)&&(0 == $checking_mclusters)) {
        # Check Width
        if ($post_line =~ m/^$matrix_width/) {
          undef @split_line;
          @split_line = split /=/,$post_line;
          # Double check 
          if ($matrix_width eq $split_line[0]) {
            $mclusters_ptr->{matrix_width} = $split_line[1]; 
            next;
          }
          else {
            print "Warning:Invalid definition for matrix width at LINE[$line_no]!\n";
          }
        }
        # Check Depth
        if ($post_line =~ m/^$matrix_depth/) {
          undef @split_line;
          @split_line = split /=/,$post_line;
          # Double check 
          if ($matrix_depth eq $split_line[0]) {
            $mclusters_ptr->{matrix_depth} = $split_line[1]; 
            next;
          }
          else {
            print "Warning:Invalid definition for matrix depth at LINE[$line_no]!\n";
          }
        }
        # Check cell_size
        if ($post_line =~ m/^$cell_size/) {
          undef @split_line;
          @split_line = split /=/,$post_line;
          # Double check 
          if ($cell_size eq $split_line[0]) {
            $mclusters_ptr->{cell_size} = $split_line[1]; 
            next;
          }
          else {
            print "Warning:Invalid definition for cell size at LINE[$line_no]!\n";
          }
        }
        # Check defined MCluster Number
        if ($post_line =~ m/^$mcluster_num/) {
          undef @split_line;
          @split_line = split /=/,$post_line;
          # Double check 
          if ($mcluster_num eq $split_line[0]) {
            $mclusters_ptr->{MCluster_num} = $split_line[1]; 
            next;
          }
          else {
            print "Warning:Invalid definition for $mcluster_num at LINE[$line_no]!\n";
          }
        }
        # Check layer
        # TODO: We should check layer should only defined ONCE!!!
        if ($post_line =~ m/^$layer/) {
          $checking_layer = 1;
          # Clear Counter
          $layer_line_cnt = 0; 
          # Check valid expected_layer_line_no
          $expected_layer_line_no = $mclusters_ptr->{matrix_width}*($mclusters_ptr->{matrix_depth}-1)*$mclusters_ptr->{cell_size};
          if (($expected_layer_line_no < 1)&&(1 != $mclusters_ptr->{matrix_width}*$mclusters_ptr->{matrix_depth})) {
            die "Error: Invalid expected_layer_line_no($expected_layer_line_no)!\nProbably caused by missing definition of $matrix_width, $matrix_depth, $cell_size before defining $layer...\n";
          }
          next;
        }
        # Check MClusters
        if ($post_line =~ m/^$mcluster/) {
          $checking_mclusters = 1;
          # Clear Counter
          $mcluster_line_cnt = 0; 
          # Check valid expected_mcluster_line_no
          $expected_mcluster_line_no = $mclusters_ptr->{matrix_width}*$mclusters_ptr->{matrix_depth};
          if ($expected_mcluster_line_no < 1) {
            die "Error: Invalid expected_mcluster_line_no($expected_mcluster_line_no)!\nProbably caused by missing definition of $matrix_width, $matrix_depth, $cell_size before defining $mcluster...\n";
          }
          next;
        }
      }
  
      # Layer FSM
      if (1 == $checking_layer) {
        # Check expected_layer_line_no
        if ($expected_layer_line_no < $layer_line_cnt) {
          die "Error: expected_layer_line_no($expected_layer_line_no) unmatch layer_line_cnt($layer_line_cnt)! Missing information for layer definition!\n";
        }
        if ($post_line =~ m/^$end/) {
          $checking_layer = 0;
          # Check expected_layer_line_no
          if ($expected_layer_line_no != $layer_line_cnt) {
            die "Error: expected_layer_line_no($expected_layer_line_no) unmatch layer_line_cnt($layer_line_cnt)! Missing information for layer definition!\n";
          }
          next;
        }
        if ($post_line =~ m/^$layer_matrix/) {
          undef @split_line;
          my ($pin) = ("I");
          #@split_line = split /=/,$post_line;
          # Get layer x,y, and double check
          if ($post_line =~ m/^$layer_matrix\[(\d+)\]\.$pin\[(\d+)\]=(\d+)/) {
            # Record des_idx, pin_idx, src_idx
            my ($des_idx,$pin_idx,$src_idx) = ($1,$2,$3);
            # Record X and Y
            my ($des_y,$des_x) = (($des_idx)%($mclusters_ptr->{matrix_width}),int($des_idx/$mclusters_ptr->{matrix_width}));
            my ($src_y,$src_x) = (($src_idx)%($mclusters_ptr->{matrix_width}),int($src_idx/$mclusters_ptr->{matrix_width}));
            # Check X range. Should be 0 < X < matrix_depth
            if (0 == $des_x) {
              print "Warning: there is no need to define zero layer at LINE[$line_no]!\n";
              next;
            }
            if ((0 > $des_x)||($des_x > ($mclusters_ptr->{matrix_depth}-1))) {
              die "Error: Invalid des_x($des_x) in LINE[$line_no]!\n"; 
            }
            # Check Y range. Should be 0 <= Y < matrix_width
            if ((0 > $des_y)||($des_y > ($mclusters_ptr->{matrix_width}-1))) {
              die "Error: Invalid des_y($des_y) in LINE[$line_no]!\n"; 
            }
            # Check X range. Should be 0 < X < matrix_depth
            if ((0 > $src_x)||($src_x > ($mclusters_ptr->{matrix_depth}-1))) {
              die "Error: Invalid src_x($src_x) in LINE[$line_no]!\n"; 
            }
            # Check Y range. Should be 0 <= Y < matrix_width
            if ((0 > $src_y)||($src_y > ($mclusters_ptr->{matrix_width}-1))) {
              die "Error: Invalid src_y($src_y) in LINE[$line_no]!\n"; 
            }
            $mclusters_ptr->{"arch"}->{"cell[$des_x][$des_y]"}->{"I[$pin_idx]"} = $src_y;
            # Check matrix content, chomp the last "," and length should be matrix_width
            #$split_line[1] =~ s/,$//; # Chomp last ","
            #my @tmp = split /,/,$split_line[1];
            #if ($#tmp != ($mclusters_ptr->{matrix_width}-1)) {
            #  die "Error: Invalid length of cross-connectivity matrix!\n";
            #}
            #my $j = 0;
            #for (my $i=0; $i<$mclusters_ptr->{matrix_width}; $i++) {
              # Valid $tmp[$i] is either 0 or 1
            #  if ((0 != $tmp[$i])&&(1 != $tmp[$i])) {
            #    die "Error: Invalid value of cross-connectivity matrix at LINE[$line_no]!\n";
            #  }
            #  if (1 == $tmp[$i]) {
            #    $mclusters_ptr->{"arch"}->{"cell[$x][$y]"}->{"I[$j]"} = $i;
            #    $j = $j + 1;
            #  }
            #}  
            # Check $j (number of input)
            #if ($j != $mclusters_ptr->{cell_size}) {
            #  die "Error: cross-connectivity matrix exceeds cell_size at LINE[$line_no]!\n";
            #}

            $layer_line_cnt = $layer_line_cnt + 1; 
          }
          else {
            print "Warning: Invalid definition of $layer_matrix in $layer at LINE($line_no)!\n";
          }
        }
        next;
      }  

      # Mcluster FSM
      if (1 == $checking_mclusters) {      
        # Check expected_mcluster_line_no
        if ($expected_mcluster_line_no < $mcluster_line_cnt) {
          die "Error: expected_mcluster_line_no($expected_mcluster_line_no) unmatch mcluster_line_cnt($mcluster_line_cnt)! Missing information for MCluster definition!\n";
        }
        if ($post_line =~ m/^$end/) {
          $checking_mclusters = 0;
          # Check expected_mcluster_line_no
          if ($expected_mcluster_line_no != $mcluster_line_cnt) {
            die "Error: expected_mcluster_line_no($expected_mcluster_line_no) unmatch mcluster_line_cnt($mcluster_line_cnt)! Missing information for MCluster definition!\n";
          }
          # Incremental Mcluster counter
          $mcluster_cnt = $mcluster_cnt + 1;
          next;
        }
        if ($post_line =~ m/^$mcluster_cell/) {
          undef @split_line;
          @split_line = split /=/,$post_line;
          # Get layer x,y, and double check
          if ($split_line[0] =~ m/^$mcluster_cell\[(\d+)\]\[(\d+)\]/) {
            # Record X and Y
            ($x,$y) = ($1,$2);
            # Check X range. Should be 0 <= X < matrix_depth
            if ((0 > $x)||($x > ($mclusters_ptr->{matrix_depth}-1))) {
              die "Error: Invalid x($x) in LINE[$line_no]!\n"; 
            }
            # Check Y range. Should be 0 <= Y < matrix_width
            if ((0 > $y)||($y > ($mclusters_ptr->{matrix_width}-1))) {
              die "Error: Invalid y($y) in LINE[$line_no]!\n"; 
            }
            # Check matrix content, chomp the last "," and length should be matrix_width
            $split_line[1] =~ s/,$//; # Chomp last ","
            my @tmp = split /,/,$split_line[1];
            if ($#tmp != ($mclusters_ptr->{cell_size})) {
              die "Error: Invalid length of MCluster cell definition at LINE[$line_no]!\n";
            }
            for (my $i=0; $i<$mclusters_ptr->{cell_size}; $i++) {
              # Valid $tmp[$i] is either $unconn or $conn
              if (($unconn ne $tmp[$i])&&($conn ne $tmp[$i])) {
                die "Error: Invalid value of MCluster cell definition at LINE[$line_no]!\n";
              }
              $mclusters_ptr->{"MCluster$mcluster_cnt"}->{"cell[$x][$y]"}->{"I[$i]"} = $tmp[$i];
            }  
            $mclusters_ptr->{"MCluster$mcluster_cnt"}->{"cell[$x][$y]"}->{"net"} = $tmp[$mclusters_ptr->{cell_size}];

            $mcluster_line_cnt = $mcluster_line_cnt + 1; 
          }
          else {
            print "Warning: Invalid definition of $mcluster_cell in $mcluster at LINE($line_no)!\n";
          }
        }
        next;
      }
    }
  }
  print "complete!\n";

  # Check mcluster_number match
  if ($mcluster_cnt != $mclusters_ptr->{MCluster_num}) {
    die "Error: Mismatch!(Expect $mclusters_ptr->{MCluster_num} MClusters, Actual $mcluster_cnt)\n"; 
  }

  close(MRPT);

  # Update Number of MClusters
  #$mclusters_ptr->{MCluster_num} = $mcluster_cnt;
   
  print "Number of MCluster: $mclusters_ptr->{MCluster_num}\n";
  
  return 1;
}

# Print models for AApack Architecture
sub gen_arch_models_pack()
{
  my ($minput_num,$moutput_num) = ($mclusters_ptr->{cell_size}*$mclusters_ptr->{matrix_width},$mclusters_ptr->{matrix_width});

  print FARCH "  <models>\n";
  print FARCH "    <model name=\"$conf_ptr->{arch_model}->{matrix_model_name}->{val}\">\n";
  print FARCH "      <input_ports>\n";
  print FARCH "        <port name=\"$conf_ptr->{arch_model}->{matrix_inport_name}->{val}\"/>\n";
  print FARCH "      </input_ports>\n";
  print FARCH "      <output_ports>\n";
  print FARCH "        <port name=\"$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\"/>\n";
  print FARCH "      </output_ports>\n";
  print FARCH "    </model>\n";
  print FARCH "  </models>\n";
}

sub gen_arch_device()
{
  my ($power_buf_size,$power_mux_trans_size);
  ($power_buf_size) = ($conf_ptr->{arch_device}->{mux_buf_size}->{val}/$SiNW_area_ratio);
  ($power_mux_trans_size) = ($conf_ptr->{arch_device}->{mux_trans_size}->{val}/$SiNW_area_ratio);

  print FARCH "  <layout auto=\"1.0\"/>\n";
  print FARCH "  <device>\n";
  print FARCH "    <sizing R_minW_nmos=\"$conf_ptr->{arch_device}->{R_minW_nmos}->{val}\" R_minW_pmos=\"$conf_ptr->{arch_device}->{R_minW_pmos}->{val}\" ipin_mux_trans_size=\"$conf_ptr->{arch_device}->{ipin_mux_trans_size}->{val}\"/>\n";
  print FARCH "    <timing C_ipin_cblock=\"$conf_ptr->{arch_device}->{C_ipin_cblock}->{val}\" T_ipin_cblock=\"$conf_ptr->{arch_device}->{T_ipin_cblock}->{val}\"/>\n";
  print FARCH "    <area grid_logic_tile_area=\"$conf_ptr->{arch_device}->{grid_logic_tile_area}->{val}\"/>\n";
  print FARCH "    <sram area=\"9\"/>\n";
  print FARCH "    <chan_width_distr>\n";
  print FARCH "      <io width=\"1.0\"/>\n";
  print FARCH "      <x distr=\"uniform\" peak=\"1.0\"/>\n";
  print FARCH "      <y distr=\"uniform\" peak=\"1.0\"/>\n";
  print FARCH "    </chan_width_distr>\n";
  print FARCH "    <switch_block type=\"wilton\" fs=\"3\"/>\n";
  print FARCH "  </device>\n";
  print FARCH "  <switchlist>\n";
  print FARCH "    <switch type=\"mux\" name=\"0\" R=\"$conf_ptr->{arch_device}->{mux_R}->{val}\" Cin=\"$conf_ptr->{arch_device}->{mux_Cin}->{val}\" Cout=\"$conf_ptr->{arch_device}->{mux_Cout}->{val}\" Tdel=\"$conf_ptr->{arch_device}->{mux_Tdel}->{val}\" mux_trans_size=\"$conf_ptr->{arch_device}->{mux_trans_size}->{val}\" buf_size=\"$conf_ptr->{arch_device}->{mux_buf_size}->{val}\" power_buf_size=\"$power_buf_size\"/>\n";
  print FARCH "  </switchlist>\n";
  print FARCH "  <segmentlist>\n";
  print FARCH "    <segment freq=\"1.0\" length=\"$conf_ptr->{arch_device}->{segment_length}->{val}\" type=\"unidir\" Rmetal=\"$conf_ptr->{arch_device}->{segment_Rmetal}->{val}\" Cmetal=\"$conf_ptr->{arch_device}->{segment_Cmetal}->{val}\">\n";
  print FARCH "    <mux name=\"0\"/>\n";
  print FARCH "    <sb type=\"pattern\">";

  for (my $i=0; $i<($conf_ptr->{arch_device}->{segment_length}->{val}+1); $i++) {
    print FARCH "1 ";
  }  

  print FARCH "</sb>\n";

  print FARCH "    <cb type=\"pattern\">";

  for (my $i=0; $i<$conf_ptr->{arch_device}->{segment_length}->{val}; $i++) {
    print FARCH "1 ";
  }  

  print FARCH "</cb>\n";
  print FARCH "    </segment>\n";
  print FARCH "  </segmentlist>\n";
  
}

sub gen_arch_complexblocks_io()
{
  my ($io_optimal) = int(2*sqrt($opt_ptr->{N_val}*$mclusters_ptr->{matrix_width})+0.5);

  # Print I/O pad first(Constraint of VPR 7)
  print FARCH "    <pb_type name=\"io\" capacity=\"$io_optimal\">\n";
  print FARCH "      <input name=\"outpad\" num_pins=\"1\"/>\n";
  print FARCH "      <output name=\"inpad\" num_pins=\"1\"/>\n";
  print FARCH "      <clock name=\"clock\" num_pins=\"1\"/>\n";
  print FARCH "      <mode name=\"inpad\">\n";
  print FARCH "        <pb_type name=\"inpad\" blif_model=\".input\" num_pb=\"1\">\n";
  print FARCH "          <output name=\"inpad\" num_pins=\"1\"/>\n";
  print FARCH "        </pb_type>\n";
  print FARCH "        <interconnect>\n";
  print FARCH "          <direct name=\"inpad\" input=\"inpad.inpad\" output=\"io.inpad\">\n";
  print FARCH "            <delay_constant max=\"4.243e-11\" in_port=\"inpad.inpad\" out_port=\"io.inpad\"/>\n";
  print FARCH "          </direct>\n";
  print FARCH "        </interconnect>\n";
  print FARCH "      </mode>\n";
  print FARCH "      <mode name=\"outpad\">\n";
  print FARCH "        <pb_type name=\"outpad\" blif_model=\".output\" num_pb=\"1\">\n";
  print FARCH "          <input name=\"outpad\" num_pins=\"1\"/>\n";
  print FARCH "        </pb_type>\n";
  print FARCH "        <interconnect>\n";
  print FARCH "          <direct name=\"outpad\" input=\"io.outpad\" output=\"outpad.outpad\">\n";
  print FARCH "            <delay_constant max=\"1.394e-11\" in_port=\"io.outpad\" out_port=\"outpad.outpad\"/>\n";
  print FARCH "          </direct>\n";
  print FARCH "        </interconnect>\n";
  print FARCH "      </mode>\n";
  print FARCH "      <fc default_in_type=\"frac\" default_in_val=\"0.15\" default_out_type=\"frac\" default_out_val=\"0.10\"/>\n";
  print FARCH "      <pinlocations pattern=\"custom\">\n";
  print FARCH "        <loc side=\"left\">io.outpad io.inpad io.clock</loc>\n";
  print FARCH "        <loc side=\"top\">io.outpad io.inpad io.clock</loc>\n";
  print FARCH "        <loc side=\"right\">io.outpad io.inpad io.clock</loc>\n";
  print FARCH "        <loc side=\"bottom\">io.outpad io.inpad io.clock</loc>\n";
  print FARCH "      </pinlocations>\n";
  print FARCH "      <gridlocations>\n";
  print FARCH "        <loc type=\"perimeter\" priority=\"10\"/>\n";
  print FARCH "      </gridlocations>\n";
  if ("on" eq $opt_ptr->{power}) {
    print FARCH "      <power method=\"ignore\"/>\n";
  }
  print FARCH "    </pb_type>\n";
  # I/O pad print over


  print FARCH "\n";
}

sub gen_arch_complexblocks_dffs()
{
  # Print DFFs 
  print FARCH "        <pb_type name=\"ff\" blif_model=\".latch\" num_pb=\"$mclusters_ptr->{matrix_width}\" class=\"flipflop\">\n"; 
  print FARCH "          <input name=\"D\" num_pins=\"1\" port_class=\"D\"/>\n"; 
  print FARCH "          <output name=\"Q\" num_pins=\"1\" port_class=\"Q\"/>\n"; 
  print FARCH "          <clock name=\"clk\" num_pins=\"1\" port_class=\"clock\"/>\n"; 
  print FARCH "          <T_setup value=\"$conf_ptr->{arch_complexblocks}->{dff_tsetup}->{val}\" port=\"ff.D\" clock=\"clk\"/>\n"; 
  print FARCH "          <T_clock_to_Q max=\"$conf_ptr->{arch_complexblocks}->{dff_tclk2q}->{val}\" port=\"ff.Q\" clock=\"clk\"/>\n"; 
  print FARCH "        </pb_type>\n"; 

  print FARCH "\n"; 
}

# Print Complex Block for Matrix Marco
sub gen_arch_complexblocks_matrix_marco()
{
  my ($mcluster_input_num,$mcluster_output_num);
  $mcluster_input_num = $mclusters_ptr->{matrix_width}*$mclusters_ptr->{cell_size};
  $mcluster_output_num = $mclusters_ptr->{matrix_width};

  # Print Matrix Marco Pb_type 
  print FARCH "        <pb_type name=\"matrix\" blif_model=\".subckt $conf_ptr->{arch_model}->{matrix_model_name}->{val}\" num_pb=\"1\">\n"; 
  print FARCH "          <input name=\"$conf_ptr->{arch_model}->{matrix_inport_name}->{val}\" num_pins=\"$mcluster_input_num\"/>\n"; 
  print FARCH "          <output name=\"$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\" num_pins=\"$mcluster_output_num\"/>\n"; 
  print FARCH "          <delay_matrix type=\"max\" in_port=\"matrix.$conf_ptr->{arch_model}->{matrix_inport_name}->{val}\" out_port=\"matrix.$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\">\n"; 

  for (my $i=0; $i<$mcluster_input_num; $i++) {
    print FARCH "            ";
    for (my $j=0; $j<$mcluster_output_num; $j++) {
      print FARCH "$conf_ptr->{arch_complexblocks}->{matrix_delay}->{val}  "; 
    }
    print FARCH "\n"; 
  }

  print FARCH "          </delay_matrix>\n"; 
  print FARCH "        </pb_type>\n"; 

}

# Print Complex Blocks for CLB, only for AAPack
sub gen_arch_complexblocks_clb_pack()
{
  my ($clb_input_num,$clb_output_num);
  my ($mcluster_input_num,$mcluster_output_num);

  $mcluster_input_num = $mclusters_ptr->{matrix_width}*$mclusters_ptr->{cell_size};
  $mcluster_output_num = $mclusters_ptr->{matrix_width};
  $clb_input_num = $opt_ptr->{I_val};
  $clb_output_num = $opt_ptr->{N_val}*$mclusters_ptr->{matrix_width};

  # Print CLB general information
  print FARCH "    <pb_type name=\"clb\">\n"; 
  print FARCH "      <input name=\"I\" num_pins=\"$clb_input_num\" equivalent=\"$conf_ptr->{arch_complexblocks}->{CLB_logic_equivalent}->{val}\"/>\n"; 
  print FARCH "      <output name=\"O\" num_pins=\"$clb_output_num\" equivalent=\"false\"/>\n"; 
  print FARCH "      <clock name=\"clk\" num_pins=\"1\"/>\n"; 
  
  # Print Sub Complex Block "BLE" 
  print FARCH "      <pb_type name=\"mble\" num_pb=\"$opt_ptr->{N_val}\">\n"; 
  print FARCH "        <input name=\"I\" num_pins=\"$mcluster_input_num\"/>\n"; 
  print FARCH "        <output name=\"O\" num_pins=\"$mcluster_output_num\"/>\n"; 
  print FARCH "        <clock name=\"clk\" num_pins=\"1\"/>\n"; 

  &gen_arch_complexblocks_matrix_marco();
  
  &gen_arch_complexblocks_dffs();
  
  # Print interconnections for BLE
  my ($ff_idx) = ($mcluster_output_num-1);
  print FARCH "        <interconnect>\n"; 
  # Clock
  print FARCH "          <complete name=\"mble_clks\" input=\"mble.clk\" output=\"ff[$ff_idx:0].clk\"/>\n"; 
  print FARCH "          <direct name=\"mble_direct\" input=\"mble.I\" output=\"matrix.$conf_ptr->{arch_model}->{matrix_inport_name}->{val}\"/>\n"; 
  print FARCH "          <direct name=\"mble_ff\" input=\"matrix.$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\" output=\"ff[$ff_idx:0].D\"/>\n"; 
 
  for (my $i=0; $i<$mclusters_ptr->{matrix_width}; $i++) {
    print FARCH "          <mux name=\"mux_ff\[$i\]\" input=\"ff\[$i\].Q matrix.$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\[$i\]\" output=\"mble.O\[$i\]\">\n";
    print FARCH "            <delay_constant max=\"$conf_ptr->{arch_complexblocks}->{mux2to1_delay}->{val}\" in_port=\"ff[$i].Q matrix.$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\[$i\]\" out_port=\"mble.O\[$i\]\"/>\n"; 
    print FARCH "          </mux>\n"; 
  }
  
  print FARCH "        </interconnect>\n"; 
  print FARCH "      </pb_type>\n"; 

  # Print interconnection for CLB
  my ($crossbar_delay) = ($conf_ptr->{arch_complexblocks}->{mux2to1_delay}->{val}*int(log($clb_input_num+$opt_ptr->{N_val}*$mcluster_output_num-1)/log(2)+1));
  my ($mble_idx) = ($opt_ptr->{N_val}-1);
  print FARCH "      <interconnect>\n"; 
  # Crossbar
  print FARCH "        <complete name=\"crossbar\" input=\"clb.I mble[$mble_idx:0].O\" output=\"mble.I\">\n"; 
  print FARCH "          <delay_constant max=\"$crossbar_delay\" in_port=\"clb.I mble[$mble_idx:0].O\" out_port=\"mble.I\"/>\n";
  print FARCH "        </complete>\n"; 
  # Clock
  print FARCH "        <complete name=\"clb_clks\" input=\"clb.clk\" output=\"mble[$mble_idx:0].clk\"/>\n"; 
  print FARCH "        <direct name=\"clb_direct\" input=\"mble[$mble_idx:0].O\" output=\"clb.O\"/>\n"; 

  print FARCH "      </interconnect>\n"; 
  
  print FARCH "      <fc default_in_type=\"frac\" default_in_val=\"0.15\" default_out_type=\"frac\" default_out_val=\"0.10\"/>\n";
  print FARCH "      <pinlocations pattern=\"spread\"/>\n"; 
  print FARCH "      <gridlocations>\n"; 
  print FARCH "        <loc type=\"fill\" priority=\"1\"/>\n";
  print FARCH "      </gridlocations>\n"; 
  print FARCH "    </pb_type>\n"; 

  print FARCH "\n"; 
}

# Print Complex Blocks for AAPack usage
sub gen_arch_complexblocks_pack()
{
  print FARCH "  <complexblocklist>\n"; 

  # Print I/O pad first
  &gen_arch_complexblocks_io();

  # Print Matrix-based Pb_type
  &gen_arch_complexblocks_clb_pack();
  print FARCH "  </complexblocklist>\n"; 
}

sub gen_arch_pack()
{
  my ($line,$post_line,$line_no);
  
  print "Generating Architecture XML($opt_ptr->{arch_file_pack_val}) for AAPack...";

  open (FARCH, "> $opt_ptr->{arch_file_pack_val}") or die "Fail to open arch_pack: $opt_ptr->{arch_file_pack_val}!\n";

  print FARCH "<!-- VPR7 Architecture for AAPack-->\n";
  print FARCH "<!-- Designed for MPACK1 -->\n";
  print FARCH "<!-- Author : Xifan TANG -->\n";
  print FARCH "<!--          EPFL LSI   -->\n";
  print FARCH "<!-- Date: $mydate -->\n";
  print FARCH "<!-- I = $opt_ptr->{I_val} -->\n";
  print FARCH "<!-- N = $opt_ptr->{N_val} -->\n";
  print FARCH "<!-- Matrix Width = $mclusters_ptr->{matrix_width} -->\n";
  print FARCH "<!-- Matrix Depth = $mclusters_ptr->{matrix_depth} -->\n";
  print FARCH "<!-- cell_size = $mclusters_ptr->{cell_size} -->\n";
  print FARCH "<architecture>\n";

  # Write <Models>
  &gen_arch_models_pack();

  # Write <Devices>
  &gen_arch_device();

  # Write <ComplexBlocks>
  &gen_arch_complexblocks_pack();

  print FARCH "</architecture>\n";

  close(FARCH);

  print "Complete.\n";
}

# Print models for VPR Architecture
sub gen_arch_models_vpr()
{
  my ($minput_num,$moutput_num) = ($mclusters_ptr->{cell_size},1);

  print FARCH "  <models>\n";
  print FARCH "    <model name=\"$conf_ptr->{arch_model}->{cell_model_name}->{val}\">\n";
  print FARCH "      <input_ports>\n";
  print FARCH "        <port name=\"$conf_ptr->{arch_model}->{cell_inport_name}->{val}\"/>\n";
  print FARCH "      </input_ports>\n";
  print FARCH "      <output_ports>\n";
  print FARCH "        <port name=\"$conf_ptr->{arch_model}->{cell_outport_name}->{val}\"/>\n";
  print FARCH "      </output_ports>\n";
  print FARCH "    </model>\n";
  print FARCH "  </models>\n";
}

# Print Complex Block for Matrix Cells
sub gen_arch_complexblocks_matrix_cells()
{
  my ($num_cell) = ($mclusters_ptr->{matrix_width}*$mclusters_ptr->{matrix_depth});
  # Print Matrix Cell Pb_type 
  print FARCH "          <pb_type name=\"cell\" blif_model=\".subckt $conf_ptr->{arch_model}->{cell_model_name}->{val}\" num_pb=\"$num_cell\">\n"; 
  print FARCH "            <input name=\"$conf_ptr->{arch_model}->{cell_inport_name}->{val}\" num_pins=\"$mclusters_ptr->{cell_size}\"/>\n"; 
  print FARCH "            <output name=\"$conf_ptr->{arch_model}->{cell_outport_name}->{val}\" num_pins=\"1\"/>\n"; 
  print FARCH "            <delay_matrix type=\"max\" in_port=\"cell.$conf_ptr->{arch_model}->{cell_inport_name}->{val}\" out_port=\"cell.$conf_ptr->{arch_model}->{cell_outport_name}->{val}\">\n"; 

  for (my $i=0; $i<$mclusters_ptr->{cell_size}; $i++) {
    print FARCH "              ";
    print FARCH "$conf_ptr->{arch_complexblocks}->{cell_delay}->{val}  "; 
    print FARCH "\n"; 
  }

  print FARCH "            </delay_matrix>\n"; 
  if ("on" eq $opt_ptr->{power}) {
    print FARCH "            <power method=\"absolute\">\n";
    print FARCH "              <dynamic_power power_per_instance=\"$conf_ptr->{arch_complexblocks}->{cell_dynamic_power}->{val}\"/>\n";
    print FARCH "              <static_power power_per_instance=\"$conf_ptr->{arch_complexblocks}->{cell_static_power}->{val}\"/>\n";
    print FARCH "            </power>\n";
  }
  print FARCH "          </pb_type>\n"; 

}

# Print Complex Blocks for CLB, only for VPR
sub gen_arch_complexblocks_clb_vpr()
{
  my ($clb_input_num,$clb_output_num);
  my ($mcluster_input_num,$mcluster_output_num);

  $mcluster_input_num = $mclusters_ptr->{matrix_width}*$mclusters_ptr->{cell_size};
  $mcluster_output_num = $mclusters_ptr->{matrix_width};
  $clb_input_num = $opt_ptr->{I_val};
  $clb_output_num = $opt_ptr->{N_val}*$mclusters_ptr->{matrix_width};

  # Print CLB general information
  print FARCH "    <pb_type name=\"clb\">\n"; 
  print FARCH "      <input name=\"I\" num_pins=\"$clb_input_num\" equivalent=\"$conf_ptr->{arch_complexblocks}->{CLB_logic_equivalent}->{val}\"/>\n"; 
  print FARCH "      <output name=\"O\" num_pins=\"$clb_output_num\" equivalent=\"false\"/>\n"; 
  print FARCH "      <clock name=\"clk\" num_pins=\"1\"/>\n"; 
  
  # Print Sub Complex Block "BLE" 
  print FARCH "      <pb_type name=\"mble\" num_pb=\"$opt_ptr->{N_val}\">\n"; 
  print FARCH "        <input name=\"I\" num_pins=\"$mcluster_input_num\"/>\n"; 
  print FARCH "        <output name=\"O\" num_pins=\"$mcluster_output_num\"/>\n"; 
  print FARCH "        <clock name=\"clk\" num_pins=\"1\"/>\n"; 
  print FARCH "        <pb_type name=\"matrix\" num_pb=\"1\">\n"; 
  print FARCH "          <input name=\"$conf_ptr->{arch_model}->{matrix_inport_name}->{val}\" num_pins=\"$mcluster_input_num\"/>\n"; 
  print FARCH "          <output name=\"$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\" num_pins=\"$mcluster_output_num\"/>\n"; 

  &gen_arch_complexblocks_matrix_cells();

  print FARCH "          <interconnect>\n"; 
  # Print Interconnection Scheme(Internal Matrix), cell[$i][$j], $i->row $j->column
  for (my $i=0; $i<$mclusters_ptr->{matrix_depth}; $i++) {
    for (my $j=0; $j<$mclusters_ptr->{matrix_width}; $j++) {
      my ($cell_num) = ($i*$mclusters_ptr->{matrix_width} + $j);
      for (my $k=0; $k<$mclusters_ptr->{cell_size}; $k++) {
        if (0 == $i) { 
          my ($input_idx) = ($j*$mclusters_ptr->{cell_size}+$k);
          print FARCH "           <direct name=\"cell[$i][$j]_input[$k]\" input=\"matrix.$conf_ptr->{arch_model}->{matrix_inport_name}->{val}\[$input_idx\]\" output=\"cell[$cell_num].$conf_ptr->{arch_model}->{cell_inport_name}->{val}\[$k\]\"/>\n"; 
        }
        else {
          my ($pred_idx) = ($mclusters_ptr->{"arch"}->{"cell[$i][$j]"}->{"I[$k]"}+($i-1)*$mclusters_ptr->{matrix_width});
          print FARCH "           <direct name=\"cell[$i][$j]_input[$k]\" input=\"cell[$pred_idx].$conf_ptr->{arch_model}->{cell_outport_name}->{val}\" output=\"cell[$cell_num].$conf_ptr->{arch_model}->{cell_inport_name}->{val}\[$k\]\"/>\n"; 
        }
      }
    }
  }
  # Print Output direct interconnections, cells.O -> matrix.O
  for (my $j=0; $j<$mclusters_ptr->{matrix_width}; $j++) {
    my ($last_layer) = ($mclusters_ptr->{matrix_depth}-1);
	my ($pred_idx) = ($mclusters_ptr->{matrix_width}*$last_layer + $j);
    print FARCH "           <direct name=\"matrix_output[$j]\" input=\"cell[$pred_idx].$conf_ptr->{arch_model}->{cell_outport_name}->{val}\" output=\"matrix.$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\[$j\]\"/>\n"; 
  }

  print FARCH "          </interconnect>\n"; 
  print FARCH "        </pb_type>\n";
  
  &gen_arch_complexblocks_dffs();
  
  # Print interconnections for BLE
  my ($ff_idx) = ($mcluster_output_num-1);
  print FARCH "        <interconnect>\n"; 
  # Clock
  print FARCH "          <complete name=\"mble_clks\" input=\"mble.clk\" output=\"ff[$ff_idx:0].clk\"/>\n"; 
  print FARCH "          <direct name=\"mble_direct\" input=\"mble.I\" output=\"matrix.$conf_ptr->{arch_model}->{matrix_inport_name}->{val}\"/>\n"; 
  print FARCH "          <direct name=\"mble_ff\" input=\"matrix.$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\" output=\"ff[$ff_idx:0].D\"/>\n"; 

  # Print MUX for DFFs
  for (my $i=0; $i<$mclusters_ptr->{matrix_width}; $i++) {
    my ($cur_cell_idx) = ($i+$mclusters_ptr->{matrix_width}*($mclusters_ptr->{matrix_depth}-1)); 
    print FARCH "          <mux name=\"mux_ff[$i]\" input=\"ff[$i].Q matrix.O[$i]\" output=\"mble.O[$i]\">\n";
    print FARCH "            <delay_constant max=\"$conf_ptr->{arch_complexblocks}->{mux2to1_delay}->{val}\" in_port=\"ff[$i].Q matrix.O[$i]\" out_port=\"mble.O[$i]\"/>\n"; 
    print FARCH "          </mux>\n"; 
  }
  
  print FARCH "        </interconnect>\n"; 
  print FARCH "      </pb_type>\n"; 

  # Print interconnection for CLB
  my ($crossbar_delay) = ($conf_ptr->{arch_complexblocks}->{mux2to1_delay}->{val}*int(log($clb_input_num+$opt_ptr->{N_val}*$mcluster_output_num-1)/log(2)+1));
  my ($mble_idx) = ($opt_ptr->{N_val}-1);
  print FARCH "      <interconnect>\n"; 
  # Crossbar
  print FARCH "        <complete name=\"crossbar\" input=\"clb.I mble[$mble_idx:0].O\" output=\"mble.I\">\n"; 
  print FARCH "          <delay_constant max=\"$crossbar_delay\" in_port=\"clb.I mble[$mble_idx:0].O\" out_port=\"mble.I\"/>\n";
  print FARCH "        </complete>\n"; 
  # Clock
  print FARCH "        <complete name=\"clb_clks\" input=\"clb.clk\" output=\"mble[$mble_idx:0].clk\"/>\n"; 
  print FARCH "        <direct name=\"clb_direct\" input=\"mble[$mble_idx:0].O\" output=\"clb.O\"/>\n"; 

  print FARCH "      </interconnect>\n"; 
  
  print FARCH "      <fc default_in_type=\"frac\" default_in_val=\"0.15\" default_out_type=\"frac\" default_out_val=\"0.10\"/>\n";
  print FARCH "      <pinlocations pattern=\"spread\"/>\n"; 
  print FARCH "      <gridlocations>\n"; 
  print FARCH "        <loc type=\"fill\" priority=\"1\"/>\n";
  print FARCH "      </gridlocations>\n"; 
  #print FARCH "      <power method=\"auto-size\">\n";
  #print FARCH "      </power>\n";
  print FARCH "    </pb_type>\n"; 

  print FARCH "\n"; 
}

# Print Complex Blocks for VPR usage
sub gen_arch_complexblocks_vpr()
{
  print FARCH "  <complexblocklist>\n"; 

  # Print I/O pad first
  &gen_arch_complexblocks_io();

  # Print Matrix-based Pb_type
  &gen_arch_complexblocks_clb_vpr();
  print FARCH "  </complexblocklist>\n"; 
}

sub gen_arch_vpr()
{
  my ($line,$post_line,$line_no);

  print "Generating Architecture XML($opt_ptr->{arch_file_vpr_val}) for VPR 7...";

  open (FARCH, "> $opt_ptr->{arch_file_vpr_val}") or die "Fail to open arch_vpr: $opt_ptr->{arch_file_vpr_val}!\n";

  print FARCH "<!-- VPR7 Architecture for VPR-->\n";
  print FARCH "<!-- Designed for MPACK1 -->\n";
  print FARCH "<!-- Author : Xifan TANG -->\n";
  print FARCH "<!--          EPFL LSI   -->\n";
  print FARCH "<!-- Date: $mydate -->\n";
  print FARCH "<!-- I = $opt_ptr->{I_val} -->\n";
  print FARCH "<!-- N = $opt_ptr->{N_val} -->\n";
  print FARCH "<!-- Matrix Width = $mclusters_ptr->{matrix_width} -->\n";
  print FARCH "<!-- Matrix Depth = $mclusters_ptr->{matrix_depth} -->\n";
  print FARCH "<!-- cell_size = $mclusters_ptr->{cell_size} -->\n";
  print FARCH "<architecture>\n";

  # Write <Models>
  &gen_arch_models_vpr();

  # Write <Devices>
  &gen_arch_device();

  # Write <ComplexBlocks>
  &gen_arch_complexblocks_vpr();

  if ("on" eq $opt_ptr->{power}) {
    my ($power_buf_size,$power_mux_trans_size);
    ($power_buf_size) = ($conf_ptr->{arch_device}->{mux_buf_size}->{val}/$SiNW_area_ratio);
    ($power_mux_trans_size) = ($conf_ptr->{arch_device}->{mux_trans_size}->{val}/$SiNW_area_ratio);
    print FARCH "  <power>\n";
    print FARCH "    <local_interconnect C_wire=\"$conf_ptr->{arch_device}->{local_interconnect_C_wire}->{val}\"/>\n";
    print FARCH "    <mux_transistor_size mux_transistor_size=\"$power_mux_trans_size\"/>\n"; 
    print FARCH "  </power>\n";
    print FARCH "  <clocks>\n";
    print FARCH "    <clock buffer_size=\"$conf_ptr->{arch_device}->{clock_buffer_size}->{val}\" C_wire=\"$conf_ptr->{arch_device}->{clock_C_wire}->{val}\"/>\n";
    print FARCH "  </clocks>\n";
  }

  print FARCH "</architecture>\n";

  close(FARCH);

  print "Complete\n";
}

sub gen_net_vpr()
{
  my ($mytab) = ("  ");
  my %net_map;
  my ($net_map_ptr) = (\%net_map);
  my ($line);
  my ($state,$next_state,$mcluster_index) = ("ST_NORMAL","ST_NORMAL",-1);
  my ($nets);

  print "Building Hash mapping nets names to MCluster Index...\n";
  
  # 1. Build a hash map net names to MCluster index	
  for (my $i=0; $i<$mclusters_ptr->{MCluster_num}; $i++) {
    my (@net_names);
    for (my $j=0; $j<$mclusters_ptr->{matrix_width}; $j++) {
      my ($last_layer) = ($mclusters_ptr->{matrix_depth}-1);
      $net_names[$j] = $mclusters_ptr->{"MCluster$i"}->{"cell[$last_layer][$j]"}->{net};
    }
    my ($nets) = join($mytab,@net_names);
   
    $nets = $nets.$mytab; 
    #print "DEBUG: NETs($nets)\n";
    $net_map_ptr->{"$nets"}->{"Mcluster_index"} = $i;
  }

  print "Generating Net_file_VPR ($opt_ptr->{net_file_out_val}) from Net_file_AAPACK ($opt_ptr->{net_file_in_val})...";
  # 2. Replace Part of *.net format file
  # Open net_file_in, read-only
  open (FNETI, "< $opt_ptr->{net_file_in_val}") or die "Fail to open input *net formate file: $opt_ptr->{net_file_in_val}!\n";
  # Open net_file_out, write-only
  open (FNETO, "> $opt_ptr->{net_file_out_val}") or die "Fail to open output *net formate file: $opt_ptr->{net_file_out_val}!\n";

  # Copy & Paste from FNETI to FNETO
  $state = "ST_NORMAL";
  while (defined($line = <FNETI>)) {
    chomp $line;
    # States : ST_NORMAL, ST_SKIP_INPUTS, ST_MODIFY_OUTPUTS, ST_SKIP_CLOCK
    # ST_NORAML: Try to match instance = "matrix"
    # ST_SKIP_INPUTS: We got matched "matrix", skip the <inputs>
    # ST_MODIFT_OUTPUTS: We got matched "matrix and skip the <inputs>, modify <outputs>
    # ST_SKIP_CLOCK: We got matched matrix, skip <inputs> and modify <outputs>, skip <clock> and add subblocks
    if ("ST_NORMAL" eq $state) {
      # Regular expression to match block name="" instance=""
      if ($line =~ m/block(\s+)name(\s*)=(\s*)\"([\w\d\[\]\_\-\&\^\;]+)\"(\s+)instance(\s*)=(\s*)\"([\w\d\[\]\_\-\&\^\;]+)\"/) {
        my ($name,$instance) = ($4,$8); 
        # Change state only when name != open
        # Replace the outputs of instance = matrix
        if (("open" ne $name)&&($instance =~ m/matrix/)) {
          # Inside a matrix, skip inputs, switch to state = ST_SKIP_INPUTS
          print FNETO "            ";
          print FNETO "<block name=\"$name\" instance=\"$instance\" mode=\"matrix\">\n";
          $next_state = "ST_SKIP_INPUTS";
          next;
        }
      }
      print FNETO "$line\n";
    }
    elsif ("ST_SKIP_INPUTS" eq $state) {
      # Match <inputs>, DEBUG
      if (($line =~ m/\<inputs\>/)&&("on" eq $opt_ptr->{debug})) {
        print "DEBUG: Match <inputs>\n";
      }
      # Match </inputs>
      if ($line =~ m/\<\/inputs\>/) {
        $next_state = "ST_MODIFY_OUTPUTS";
        $mcluster_index = -1;
      }
      print FNETO "$line\n";
    }
    elsif ("ST_MODIFY_OUTPUTS" eq $state) {
      my ($line_copy) = ($line);
      # Determine the index of MCluster by comparing net_names!
      # Match <port name=""> net_names </port>
      if ($line_copy =~ m/<port(\s+)name(\s*)=(\s*)\"$conf_ptr->{arch_model}->{matrix_outport_name}->{val}\">([\d\w\_\s\-\^\&\;\[\]]+)<\/port>/) {
        ($nets) = ($4);
      }
      $line_copy = $line;
      # Match </outputs> and print modified outputs
      if ($line_copy =~ m/\<\/outputs\>/) {
        my ($format_nets);
        $next_state = "ST_SKIP_CLOCK";
        # Check Valid MCluster_index
        my @net_names = split /\s+/,$nets;
        for (my $i = 0; $i < ($#net_names+1); $i++) {
          $format_nets = $format_nets."$net_names[$i]$mytab";
        }
        if (exists($net_map_ptr->{"$format_nets"}->{"Mcluster_index"})) {
          $mcluster_index = $net_map_ptr->{"$format_nets"}->{"Mcluster_index"};
        }
        else {
          die "Error: Invalid Net names($format_nets)\n";
        }

        print FNETO "            ";
        print FNETO "<outputs>\n";
        my ($last_layer) = ($mclusters_ptr->{matrix_depth}-1);
        print FNETO "              <port name=\"O\">";
        for (my $i = 0; $i < $mclusters_ptr->{matrix_width}; $i++) {
	      my ($pred_idx) = ($mclusters_ptr->{matrix_width}*$last_layer + $i);
          print FNETO "cell[$pred_idx].$conf_ptr->{arch_model}->{cell_outport_name}->{val}\-\>matrix_output[$i]  ";
        }
        print FNETO "</port>\n";
        print FNETO "            ";
        print FNETO "</outputs>\n";
        print FNETO "            ";
        print FNETO "<clocks>\n";
        print FNETO "            ";
        print FNETO "</clocks>\n";
      }
    }
    elsif ("ST_SKIP_CLOCK" eq $state) {
      # Match <clocks> and print subblocks
      if ($line =~ m/\<\/clocks\>/) {
        $next_state = "ST_NORMAL";
       
        # Check mcluster_index
        if (-1 == $mcluster_index) {
          die "Error: Invalid mcluster index: $mcluster_index!\n";
        }
        # Print subblocks 
        my ($num_cell) = ($mclusters_ptr->{matrix_width}*$mclusters_ptr->{matrix_depth});
        for (my $i = 0; $i < $mclusters_ptr->{matrix_depth}; $i++) {
          for (my $j = 0; $j < $mclusters_ptr->{matrix_width}; $j++) {
            my ($cell_idx) = $i*$mclusters_ptr->{matrix_width} + $j; 
            # Fill block_name and instance_name
            my ($block_name,$instance_name) = ($mclusters_ptr->{"MCluster$mcluster_index"}->{"cell[$i][$j]"}->{net},"cell[$cell_idx]");
            print FNETO "            ";
            print FNETO "<block name=\"$block_name\" instance=\"$instance_name\">\n";
            # Check if this block has been used.
            if ("open" ne $block_name) {
              # Print Input Ports, Output Ports, ignore clocks 
              print FNETO "            ";
              print FNETO "  <inputs>\n";
              print FNETO "            ";
              print FNETO "    <port name=\"$conf_ptr->{arch_model}->{cell_inport_name}->{val}\">";
              for (my $k = 0; $k < $mclusters_ptr->{cell_size}; $k++) {
                if ("unconn" eq $mclusters_ptr->{"MCluster$mcluster_index"}->{"cell[$i][$j]"}->{"I[$k]"}) {
                  print FNETO "open ";
                }
                elsif (0 == $i) {
                  my ($tmp_idx) = ($j*$mclusters_ptr->{cell_size}+$k);
                  print FNETO "matrix[0].I[$tmp_idx]\-\>cell[$i][$j]_input[$k] ";
                }
                else {
                  my ($pred_idx) = ($mclusters_ptr->{"arch"}->{"cell[$i][$j]"}->{"I[$k]"}+($i-1)*$mclusters_ptr->{matrix_width});
                  print FNETO "cell[$pred_idx].$conf_ptr->{arch_model}->{cell_outport_name}->{val}\-\>cell[$i][$j]_input[$k] ";
                }
              }
              print FNETO "</port>\n";
              print FNETO "            ";
              print FNETO "  </inputs>\n";
              print FNETO "            ";
              print FNETO "  <outputs>\n";
              print FNETO "            ";
              print FNETO "    <port name=\"$conf_ptr->{arch_model}->{cell_outport_name}->{val}\">";
              print FNETO "$block_name ";
              print FNETO "</port>\n";
              print FNETO "            ";
              print FNETO "  </outputs>\n";
              print FNETO "            ";
              print FNETO "  <clocks>\n";
              print FNETO "            ";
              print FNETO "  </clocks>\n";
            } 
            print FNETO "            ";
            print FNETO "</block>\n";
          }
        }
      }  
    }
    $state = $next_state;
    if ("on" eq $opt_ptr->{debug}) {
      print "DEBUG: Current State=$state\n";
    }
  }

  # Close files
  close(FNETI);
  close(FNETO);

  #print "Generate Net_file_VPR \($opt_ptr->{net_file_out_val}\) from Net_file_AAPACK \($opt_ptr->{net_file_in_val}\) Complete\n";
  print "Complete\n";
}

# Main Program
sub main()
{
  # Read Options. All options stored in opt_ptr
  &opts_read();
 
  # Read basic configuration file. All confs stored in conf_ptr
  &read_conf();

  # Read mpack report file
  &read_mpack1_rpt();
 
  # Complete tasks according to selected mode 
  if ("pack_arch" eq $opt_ptr->{"mode_val"}) {
    # Generate Architecture XML for AAPack
    &gen_arch_pack(); 
  }
  elsif ("m2net" eq $opt_ptr->{"mode_val"}) {
    # Generate Architecture XML for VPR
    &gen_arch_vpr();
    # Generate Net for VPR
    &gen_net_vpr();
  }
  else {
    die "Error: Invalid mode selected!\n";
  }

}

&main();
exit(0);
