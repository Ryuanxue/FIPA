#!/usr/bin/perl

my $base = 0;
my $max = -1;

sub renumber_node {
    my($n) = @_;
    return 0 if $n == 0;
    return -1 if $n == -1;
    $n += $base;
    $max = $n if $n > $max;
    return $n;
}

for my $f (@ARGV) {
    open(IN, "<$f");
    while (<IN>) {
	die unless /^(\d+) (\d+|-1) (\d+) ([0-9a-f:]+)/;
	my($from, $to, $capacity, $ident) = ($1, $2, $3, $4);
	$from = renumber_node($from);
	$to = renumber_node($to);
	print "$from $to $capacity $ident\n";
    }
    close IN;
    $base = $max + 1;
}
