#!usr/bin/perl -w
use strict;
use Cwd;
#use Shell;
use FileHandle;
#Use the time
use Time::gmtime;

my $arch_file;
my $new_arch_file;
my $overwrite = "TRUE";
my $keyword = "OPENFPGAPATHKEYWORD";
my $default_keyword = "TRUE";
my $change_to;
my $folder_top = "OpenFPGA";

sub print_usage()
{
	print "Usage:\n";
	print "      perl <script_name.pl> [-options]\n";
	print "      Options:(Mandatory!)\n";
	print "              -i <input_architecture_file_path>\n";
	print "      Options:(Optional)\n";
	print "              -o <output_architecture_file_path>\n";
	print "              -k <keyword> <new_value>\n";
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
				$arch_file = $ARGV[$iargv+1];
				$iargv++;
			} elsif ("-o" eq $ARGV[$iargv]){
				$new_arch_file = $ARGV[$iargv+1];
				$overwrite = "FALSE";
				$iargv++;
			} elsif ("-k" eq $ARGV[$iargv]){
				$keyword = $ARGV[$iargv+1];
				$change_to = $ARGV[$iargv+2];
				$default_keyword = "FALSE";
				$iargv++;
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
	my ($arch) = @_;
	open(F, $arch);
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
	for(my $count = 0; $count < ($#folders -1); $count++){
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

sub rewrite_file($ $)
{
	my ($arch, $template) = @_;
	open(IN, '<'.$template);
	open(OUT, '>'.$arch);

	if($default_keyword eq "TRUE"){
		my $myPath = &findPath();
		while(<IN>){
			$_ =~ s/$keyword/$myPath/g;
			print OUT $_;
		}
	} else {
		while(<IN>){
			$_ =~ s/$keyword/$change_to/g;
			print OUT $_;
		}
	}
	return;
}

sub main()
{
	&opts_read();
	my $rewrite_needed = &rewriting_required_check($arch_file);
	if($rewrite_needed == 1){
		if($overwrite eq "TRUE"){
			my $template_file = &save_original($arch_file);
			&rewrite_file($arch_file, $template_file);
		} else {
			&rewrite_file($new_arch_file, $arch_file);
		}
	}
	return;
}

&main();
exit(1);
