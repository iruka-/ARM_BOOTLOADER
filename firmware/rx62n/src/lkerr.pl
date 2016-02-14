#!/usr/bin/perl

my %hash;

sub readerr() {
	my $line;
	my $id;
	my @idbuf;
	
	open(FILE,$ARGV[0]);
	while(<FILE>) {
		$line = $_;
		$line =~ s/\n//;
		$line =~ s/\r//;
		if($line =~ "undefined reference to") {
			@idbuf = split(/\`/,$line);
			$id = $idbuf[1];
			$id =~ s/\n//;
			$id =~ s/'//;
			if($id ne '') {
				$hash{$id} = $id;
			}
		}
	}
	close(FILE);
}


sub	main2()
{
	my $line;
	my $id;
	my @idbuf;

	&readerr();
	foreach $line( keys(%hash) ) {
#		print $line . "\n";
#		$line =~ s/^_//;
		print "int $line" . "(void)\n";
		print "{\n";
		print "	return 0;\n";
		print "}\n\n";
	}
}

sub	main()
{
	my $line;
	my $id;
	my @idbuf;

	&readerr();
	foreach $line( sort(keys(%hash)) ) {
#		print $line . "\n";
#		$line =~ s/^_//;
		$head = "int $line" . "(void)";
		$tablen = 36 - length($head);
		$tab = "\t";
		while($tablen>0) {
			$tab .= "\t";
			$tablen -= 4;
		}
		print $head . $tab . "{U(\"" . $line . "\");return 0;}\n";
	}
}

&main();

