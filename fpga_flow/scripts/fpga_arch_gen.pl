#!usr/bin/perl -w
use strict;
#use Shell;
use Time::gmtime;
use Switch;
use File::Path;
use Cwd;

my $mydate = gmctime();
my $cwd = getcwd();

sub gen_fpga_arch($ $)
{
  my ($k,$n) = @_;
  my ($arch_file) = ("tmp.xml");
  my ($i) = int(0.5+$k*($n+1)/2);
  print "K=$k N=$n I=$i\n";
  
  my ($seq_out_up) = (2*$n-1);

  my %ble_h;
  my $ble_ptr = \%ble_h;

  my @comb;
  my @seq;
  my ($j);

  for ($j=0; $j<$k*$n; $j++) {
    my ($idx) = int($j/$k);
    print "idx=$idx";
    my ($input) = $j%$k;
    print " input=$input\n";
    $ble_ptr->{"ble_in$j"}->{ble_idx} = $idx;
    $ble_ptr->{"ble_in$j"}->{ble_input_idx} = $input;
    if ($input < $idx) {
      $ble_ptr->{"ble_in$j"}->{comb_in} = $input;
    }
    else {
      $ble_ptr->{"ble_in$j"}->{comb_in} = -1;
    }
    $ble_ptr->{"ble$idx"}->{"input$input"}->{idx} = $j;
  }

  my ($iseq,$icomb) = (0,0);
  for (my $ible=0; $ible<$n; $ible++) {
    for (my $in=0; $in<$ible; $in++) {
      if ($in < $ible) {
        $comb[$icomb] = $ble_ptr->{"ble$ible"}->{"input$in"}->{idx};
        $icomb++;
      }
    }
    for (my $in=$ible; $in<$k; $in++) {
      if ($in < $k) {
        $seq[$iseq] = $ble_ptr->{"ble$ible"}->{"input$in"}->{idx};
        $iseq++;
      }
    }
  }

  open (FARCH," > $arch_file") or die "Fail to create $arch_file";
  print FARCH "<pb_type name=\"clb\">\n";
  print FARCH "  <input name=\"I\" num_pins=\"$i\" equivalent=\"true\"/>\n";
  print FARCH "  <output name=\"O\" num_pins=\"$n\" equivalent=\"false\"/>\n";
  print FARCH "  <clock name=\"clk\" num_pins=\"1\"/>\n";
  print FARCH "  <interconnect>\n"; 
  print FARCH "  <complete name=\"clks\" input=\"clb.clk\" output=\"ble.clk\"/>\n";
  print FARCH "  <complete name=\"crossbar_in0\" input=\"clb.I ble.out[$seq_out_up:$n]\" output=\"";
  foreach my $tmp(@seq) {
    print FARCH "ble.in[$tmp] ";
  }
  print FARCH "\">\n";
  print FARCH "  <delay_constant max=\"9.5e-11\" in_port=\"clb.I\" out_port=\"";
  foreach my $tmp(@seq) {
    print FARCH "ble.in[$tmp] ";
  }
  print FARCH "\"/>\n";
  print FARCH "  <delay_constant max=\"7.5e-11\" in_port=\"ble.out[$seq_out_up:$n] \" out_port=\"";
  foreach my $tmp(@seq) {
    print FARCH "ble.in[$tmp] ";
  }
  print FARCH "\"/>\n";
  print FARCH "</complete>\n";

  my ($imux) = (0);
  foreach my $tmp(@comb) {
    print FARCH "  <complete name=\"mux$imux\" input=\"clb.I ble.out[$seq_out_up:$n] ble.out[".$ble_ptr->{"ble_in$tmp"}->{comb_in}."]\" output=\"ble.in[$tmp]\">\n";
    print FARCH "    <delay_constant max=\"9.5e-11\" in_port=\"clb.I\" out_port=\"ble.in[$tmp]\"/>\n";
    print FARCH "    <delay_constant max=\"7.5e-11\" in_port=\"ble.out[$ble_ptr->{\"ble_in$tmp\"}->{comb_in}] \" out_port=\"ble.in[$tmp]\"/>\n";
    print FARCH "  </complete>\n";
    $imux++;
  }
  for (my $i=0; $i<$n; $i++) {
    my $j = $i + $n;
    print FARCH "  <mux name=\"mux$imux\" input=\"ble.out[$i] ble.out[$j]\" output=\"clb.O[$i]\">\n";
    print FARCH "    <delay_constant max=\"9.5e-11\" in_port=\"ble.out[$i]\" out_port=\"clb.O[$i]\"/>\n";
    print FARCH "    <delay_constant max=\"7.5e-11\" in_port=\"ble.out[$j]\" out_port=\"clb.O[$i]\"/>\n";
    print FARCH "  </mux>\n";
    $imux++;
  }
  print FARCH "  </interconnect>\n"; 


}

sub main()
{
  my ($k,$n) = (6,7);
  &gen_fpga_arch($k,$n);
}

&main();
