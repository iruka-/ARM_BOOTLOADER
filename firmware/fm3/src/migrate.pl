#!/usr/bin/perl

my %hash;

my $file1='../common/mb9bf618t.h';

my $file2='../common/mb9bf506r.h';

# =====================================================
#      
# =====================================================
sub loadfile()
{
	my ($file)=@_;
	my @buf;
	open(FILE,$file) || die("Can't open file $file");
	@buf = <FILE>;
	close(FILE);
	return @buf;
}

sub remcut()
{
	my ($line)=@_;
	$line =~ s/\n//;
	$line =~ s/\r//;
	$line =~ s|/\*.*\*/||;

	return $line;
}


sub read_line1()
{
	my ($line)=@_;
	my $def1;
	my $val1;
	$line = &remcut($line);
	my @row = split(/[ \t]+/,$line);
	if($row[0] eq '#define') {
		$def1 = $row[1];
		shift @row;
		shift @row;
		$def2 = join(/ /,@row);
		$hash{$def2}=$def1;
	}
}

sub read_line2()
{
	my ($line)=@_;
	my $def1;
	my $val1;
	$line = &remcut($line);
	my @row = split(/[ \t]+/,$line);
	if($row[0] eq '#define') {
		$def1 = $row[1];
		shift @row;
		shift @row;
		$def2 = join(/ /,@row);
		if( $hash{$def2} ne '') {
		if( $hash{$def2} ne $def1) {
			print '#define ' . $def1 . "\t" . $hash{$def2} . "\n";
		}
		}
	}
}

sub main()
{
	my $line;
	my @buf1=&loadfile($file1);
	foreach $line(@buf1) {
		&read_line1($line);
	}

	my @buf2=&loadfile($file2);
	foreach $line(@buf2) {
		&read_line2($line);
	}
}


&main();

