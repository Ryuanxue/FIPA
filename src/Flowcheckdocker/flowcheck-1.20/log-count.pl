#!/usr/bin/perl

use strict;

my %addr2name;
my %loc_count;

while (<>) {
    if (/  at 0x([0-9A-Fa-f]+): (.*)/) {
	$addr2name{hex $1} = $2;
	#print "The name of $1 is $2\n";
    } elsif (/^At ([0-9A-Fa-f]{8}).* bits leaked.* \((\d+) bits\)/) {
	$loc_count{$1} += $2;
	#print "Adding $2 bits for $1\n";
    } elsif (/^Explicit word leaked:.*\((\d+) bits\) at (.*)$/) {
	$loc_count{"preemptive-$2"} += $1;
    } elsif (/^At ([0-9A-Fa-f]{8}).* Leaked a bit/) {
	$loc_count{$1}++;
	#print "Adding one bit for $1\n";
    } elsif (/^Leaking (\d+) bits via write/) {
	$loc_count{"direct"} += $1;
    } else {
	chomp;
	#print "Skipping unparsed <$_>\n";
    }
}

my $total = 0;

for my $loc (sort keys %loc_count) {
    my $loc_n = hex $loc;
    print "$loc_count{$loc} $loc $addr2name{$loc_n}\n";
    $total += $loc_count{$loc};
}

print "$total total\n";
