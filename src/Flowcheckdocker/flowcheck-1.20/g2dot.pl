#!/usr/bin/perl

print "digraph G {\n";
while (<>) {
    /^(\d+) (-?\d+) (\d+) ([0-9a-f:]+) (.*)$/ or next;
    my($u, $v, $capacity, $loc, $info) = ($1, $2, $3, $4, $5);
    my $line = "";
    if ($info =~ /\([^)]*:(\d+)\)/) {
	$line = " l$1";
    }
    print qq/$u -> $v [label="$capacity$line"]\n/;
}
print "}\n";
