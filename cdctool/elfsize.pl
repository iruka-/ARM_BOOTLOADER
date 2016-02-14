#!/usr/bin/perl

use strict;

#my $cppfilt = 'c++filt';
my $cppfilt = '';

my $infile  = 'build/CQSTARM.elf';

sub help()
{
	exit 0;
}

my $total = 0;
my $opt_t = 0;
my $opt_d = 0;
my $opt_r = 0;
my $opt_b = 0;
my $opt_c = 0;
my $opt_x = 0;
my $opt_m = 0;
my $opt_a = 0;
my $section_f=0;
my $opt_nd = 0;
my @out;

my $opt_xflag = 0;  
my $opt_x_out = ''; 
my $opt_t_class = '';
my $opt_t_size  =  0;

sub loadfile()
{
	my ($file)=@_;
	my @buf;
	open(FILE,$file) || die("Can't open file $file");
	@buf = <FILE>;
	close(FILE);
	return @buf;
}
sub print_out()
{
	my $line;
	@out = sort(@out);
	foreach $line(@out) {
		print $line;
	}

	if( $opt_t_size != 0) {
		printf("\n=== Class size = %8d (0x%x)\n",$opt_t_size,$opt_t_size);
	}
}
sub	shortcut()
{
	my ($path) = @_;
	my $i = rindex($path,'..');
	if($i<0) {return $path;}
	return substr($path,$i);
}
sub print_section()
{
	my @row=@_;
	my $offset = hex($row[0]) + $opt_a;
	my $size   = hex($row[4]);
	if($opt_t_class ne '') {
		if(index( $row[5],$opt_t_class ) == 0) {
			my $s = sprintf("%-8s %08x %10d %s\n", 
						$row[3] , $offset , $size , $row[5] );	
			push @out,$s;
			$opt_t_size += $size;
		}
		return;
	}

	if($size >= $opt_m) {

		my $s = sprintf("%-8s %08x %10d %s\n", 
						$row[3] , $offset , $size , $row[5] );	
		push @out,$s;

	}
}
sub print_verbose()
{
	my ($line)=@_;
	my ($sect,$size,$bytes);
	my @row = split(/[ \t]+/,$line);
	my $s = $row[3];

	if($s =~ /^[.]text/ ) {
		if($opt_t) {&print_section(@row);}
	}
	if($s =~ /^[.]data/ ) {
		if($opt_d) {&print_section(@row);}
	}
	if($s =~ /^[.]rodata/ ) {
		if($opt_r) {&print_section(@row);}
	}
	if($s =~ /^[.]bss/ ) {
		if($opt_b) {&print_section(@row);}
	}
	if($s =~ /^COMMON/ ) {
		if($opt_c) {&print_section(@row);}
	}
}
sub print_sym()
{
	my ($line)=@_;
	my ($sect,$vma,$size,$offset,$bytes);
	if($line =~ /^Sections:/) {
		$section_f = 1;
	}
	if($line =~ /^SYMBOL TABLE:/) {
		$section_f = 2;
	}
	if($section_f==1) {
		my @row = split(/[ \t]+/,$line);
		if($row[2] =~ /^\./) {
			$sect = $row[2] ;
			$size = hex( $row[3] );
			$vma  = hex( $row[4] );
			$offset = hex( $row[1] ) + $opt_a;
			    if($vma) {
					printf("%4s %22s 0x%08x %8d 0x%06x\n", $row[1] ,$sect ,$vma ,$size,$size);
					$total += $size;
				}
		}
	}
	if($section_f==2) {
		&print_verbose($line);
	}
}
sub print_map()
{
	my ($line)=@_;
	if($line =~ /^[.][a-zA-Z0-9]+ / ) {
		my @row=split(/[ ]+/ , $line);
		my $size = hex($row[2]);  # ここで0x93000 などのヘキサを数値にする.

		printf("%s %8d\n", $line , $size);
		$total += $size;
		return ;
	}

	if($line =~ /^ [.]text / ) {
		if($opt_t) {&print_section($line);}
	}
	if($line =~ /^ [.]data / ) {
		if($opt_d) {&print_section($line);}
	}
	if($line =~ /^ [.]rodata / ) {
		if($opt_r) {&print_section($line);}
	}
	if($line =~ /^ [.]bss / ) {
		if($opt_b) {&print_section($line);}
	}
	if($line =~ /^ COMMON / ) {
		if($opt_c) {&print_section($line);}
	}
}

sub read_sym()
{
	my ($file)=@_;
	my $line;
	my $id;
	my @idbuf;

	my @buf = `LANG=C arm-none-eabi-objdump -x $file`;

	print  "# No.           SectionName     VMA      Size   SizeHex\n";
	foreach $line(@buf) {
		$line =~ s/\n//;
		&print_sym($line);
	}
	print  "#----------------------------------------------\n";
	printf("#                     total =          %8d (0x%x) bytes\n",$total,$total);
	close(FILE);
}
sub set_max_size()
{
	my ($opt)=@_;
	$opt = substr($opt,2);
	$opt_m = hex($opt);
}
sub set_err_address()
{
	my ($opt)=@_;
	$opt = substr($opt,2);
	$opt_x = hex($opt);
	$opt_t = 1;

	if($infile !~ /elf$/) {
		$opt_a = 0x08003000;
	}
}
sub class_name_to_label()
{
	my ($class)=@_;
	my $n = length($class);
	if( $opt_nd ) { return $class;}
	if( $class eq 'std') {
		return '_ZSt';
	}
	return sprintf( "_ZN%d%s" , $n , $class );
	
}

sub set_class_string()
{
	my ($opt)=@_;
	$opt = substr($opt,2);
	$opt_t_class = &class_name_to_label($opt);
	$opt_t = 1;
}

sub	main()
{
	my $option;
	foreach $option (@ARGV) {
		if($option eq '-t') {$opt_t = 1;next;}
		if($option eq '-d') {$opt_d = 1;next;}
		if($option eq '-r') {$opt_r = 1;next;}
		if($option eq '-b') {$opt_b = 1;next;}
		if($option eq '-c') {$opt_c = 1;next;}
		if($option eq '-nd'){$opt_nd =1;next;}
		if($option eq '-h') {&help();}
		if($option eq '-?') {&help();}
		if($option eq '--help') {&help();}
		if($option =~ '^-m') {&set_max_size($option);next;}
		if($option =~ '^-x') {&set_err_address($option);next;}
		if($option =~ '^-t') {&set_class_string($option);next;}
		if(substr($option,0,1) ne '-') {
			$infile = $option;  # 入力ファイル名が指定された.
		}
	}

	if(($cppfilt ne '')&&($opt_nd==0)) {
		open(STDOUT,"| $cppfilt");
	}
	&read_sym($infile);
	&print_out();

	if(($cppfilt ne '')&&($opt_nd==0)) {
		close(STDOUT);
	}
}

&main();



