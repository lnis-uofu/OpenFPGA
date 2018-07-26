#!/usr/bin/perl -w
use strict;
#use Shell;

my $i;

sub main{
  for ($i=3;$i<7;$i++) { 
    my $pid = fork();
    if (0 == $pid) { 
      my $n = $i + 1;
      return `perl fpga_flow.pl -conf ./configs/K$i\_N$n\_22nm_new.conf -benchmark ./benchmarks/circuits.txt -rpt K$i\_N$n\_22nm_new_full.csv -N $n -K $i`;
    }
  }
  #for ($i=3;$i<7;$i++) { 
  #  my $pid = fork();
  #  if (0 == $pid) { 
  #    my $n = $i + 1;
  #    return `perl fpga_flow.pl -conf ./configs/K$i\_N$n\_22nm.conf -benchmark ./benchmarks/circuits.txt -rpt K$i\_N$n\_22nm.csv -N $n -K $i`;
  #  }
  #}
  #for ($i=1;$i<7;$i++) { 
  #  my $pid = fork();
  #  if (0 == $pid) { 
  #    return `perl fpga_flow.pl -conf ./configs/K3M2_N$i\_22nm.conf -benchmark ./benchmarks/circuits.txt -rpt K3M2_N$i\_22nm.csv -N $i -K 3`;
  #  }
  #}
  #for ($i=1;$i<11;$i++) { 
  #  my $pid = fork();
  #  if (0 == $pid) {
  #    return `perl fpga_flow.pl -conf ./configs/K6_N$i\_22nm.conf -benchmark ./benchmarks/circuits.txt -rpt K6_N$i\_22nm.csv -N $i -K 6`;
  #  }
  #}
  #wait(-1);
}

&main();

