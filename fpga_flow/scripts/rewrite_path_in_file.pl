#!usr/bin/perl -w
use strict;
use Cwd;
#use Shell;
use FileHandle;
#Use the time
use Time::gmtime;

my $my_file;
my $keyword = "OPENFPGAPATHKEYWORD";
my $folder_top = "OpenFPGA";

sub print_usage()
{
	print "Usage:\n";
	print "      perl <script_name.pl> [-options]\n";
	print "      Options:(Mandatory!)\n";
	print "              -i <input_file_path>\n";
	print "\n";
	return;
}

sub opts_read()
{
	if ($#ARGV == -1){
		print "Error: Not enough input argument!\n";
		&print_usage();
		exit(1); 
	} else {
		for (my $iargv = 0; $iargv < $#ARGV+1; $iargv++){
			if ("-i" eq $ARGV[$iargv]){ 
				$my_file = $ARGV[$iargv+1];
				$iargv++;
			} else {
				die "WRONG ARGUMENT";
			}
		}
	}
	return;
}

sub rewriting_required_check($)
{
	my ($file) = @_;
	open(F, $file);
	my @lines=<F>;
	close F;
	my $grep_result = grep ($keyword, @lines);
	if($grep_result >= 1){
		print "Rewrite needed\n";
		return 1;
	} else {
		print "Rewrite NOT needed\n";
		return 0;
	}
}

sub save_original($)
{
	my ($template) = @_;
	my $renamed_template = "$template".".bak";
	rename($template, $renamed_template);
	
	return $renamed_template;	
}

sub findPath(){
	my $path;
	my $dir = cwd;
	my @folders = split("/", $dir);
	for(my $count = 0; $count < $#folders; $count++){
		if($folders[$count] eq ""){
		} else {
			$path = "$path"."/"."$folders[$count]";
			if($folders[$count] eq $folder_top){
				print "$path\n";
				return $path;
			}
		}
	}
	die "ERROR: Script launched from the outside of the $folder_top folder!\n";
}

sub create_new($ $)
{
	my ($file, $template) = @_;
	my $myPath = &findPath();
	open(IN, '<'.$template);
	open(OUT, '>'.$file);
	while(<IN>){
		$_ =~ s/$keyword/$myPath/g;
		print OUT $_;
	}
	return;
}

sub main()
{
	&opts_read();
	my $rewrite_needed = &rewriting_required_check($my_file);
	if($rewrite_needed == 1){
		my $template_file = &save_original($my_file); 
		&create_new($my_file, $template_file);
	}
	return;
}
 
&main();
